use datatypes::*;
use futures::prelude::*;
use futures::select;
use std::convert::TryInto;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Inputs
    let mut congo_change_stream = workspace
        .subscribe(&String::from("/congo").try_into().unwrap())
        .await
        .unwrap();

    // Outpus
    let ohio_resource: &str = "/ohio";

    println!("Monaco: Starting loop");
    loop {
        select!(
            change = congo_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let twist = deserialize_twist(buf.contiguous().as_slice()).unwrap();
                        println!("Monaco: Received Twist from /congo, putting value to {}", ohio_resource);
                        workspace
                            .put(&ohio_resource.try_into().unwrap(), twist.linear.unwrap().x.into())
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
