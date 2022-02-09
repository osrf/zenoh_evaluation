use datatypes::*;
use futures::prelude::*;
use futures::select;
use std::convert::TryInto;
use std::env;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7519"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let columbia_resource_path = format!("/{}/columbia", robot_number);
    let mut change_stream = workspace
        .subscribe(&columbia_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    let colorado_resource_path = format!("/{}/colorado", robot_number);

    println!("Taipei: Starting loop");
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
                        let image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Taipei: Received image of {} bytes from {}, putting it to {}", image_size, columbia_resource_path, colorado_resource_path);
                        let buf = serialize_image(&image);
                        let value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(buf),
                        };
                        workspace
                            .put(&colorado_resource_path.clone().try_into().unwrap(), value)
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
