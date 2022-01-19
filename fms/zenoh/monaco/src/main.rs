use datatypes::*;
use futures::prelude::*;
use futures::select;
use std::convert::TryInto;
use std::env;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Inputs
    let congo_resource_path = format!("/{}/congo", robot_number);
    let mut congo_change_stream = workspace
        .subscribe(&congo_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Outpus
    let ohio_resource_path = format!("/{}/ohio", robot_number);

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
                        println!("Monaco: Received Twist from {}, putting value to {}", congo_resource_path, ohio_resource_path);
                        workspace
                            .put(&ohio_resource_path.clone().try_into().unwrap(), twist.linear.unwrap().x.into())
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
