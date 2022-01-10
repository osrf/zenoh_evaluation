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
        .subscribe(&String::from("/amazon").try_into().unwrap())
        .await
        .unwrap();
    let output_resource: &str = "/tigris";

    println!("Lyon: Starting loop");
    loop {
        select!(
            change = change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let data = change.value.unwrap();
                        println!("Lyon: Received value, putting it to resource {}", output_resource);
                        workspace
                            .put(&output_resource.try_into().unwrap(), data.into())
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
