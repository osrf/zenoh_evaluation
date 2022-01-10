use futures::prelude::*;
use futures::select;
use std::convert::TryInto;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let mut change_stream = workspace
        .subscribe(&String::from("/arkansas").try_into().unwrap())
        .await
        .unwrap();

    println!("Arequipa: Starting loop");
    loop {
        select!(
            change = change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Arequipa: Received value");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
