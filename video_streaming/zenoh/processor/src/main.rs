use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let mut change_stream = workspace
        .subscribe(&String::from("/camera").try_into().unwrap())
        .await
        .unwrap();
    let output_resource: &str = "/processed";

    loop {
        select!(
            change = change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let image_size = buf.len();
                        let mut image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Processor: Received image of {} bytes from /camera, putting it to {}", image_size, output_resource);
                        image.header = random();
                        let buf = serialize_image(&image);
                        let value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(buf),
                        };
                        workspace
                            .put(&output_resource.try_into().unwrap(), value)
                            .await
                            .unwrap();
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
