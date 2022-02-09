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

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7511"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let amazon_resource_path = format!("/{}/amazon", robot_number);
    let mut change_stream = workspace
        .subscribe(&amazon_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    let tigris_resource_path = format!("/{}/tigris", robot_number);

    println!("Lyon: Starting loop");
    loop {
        select!(
            change = change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let data = change.value.unwrap();
                        println!("Lyon: Received value from {}, putting it to resource {}", amazon_resource_path, tigris_resource_path);
                        workspace
                            .put(&tigris_resource_path.clone().try_into().unwrap(), data.into())
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
