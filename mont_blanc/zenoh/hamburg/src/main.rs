use futures::prelude::*;
use futures::select;
use std::convert::TryInto;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let mut tigris_change_stream = workspace
        .subscribe(&String::from("/tigris").try_into().unwrap())
        .await
        .unwrap();
    let mut ganges_change_stream = workspace
        .subscribe(&String::from("/ganges").try_into().unwrap())
        .await
        .unwrap();
    let mut nile_change_stream = workspace
        .subscribe(&String::from("/nile").try_into().unwrap())
        .await
        .unwrap();
    let mut danube_change_stream = workspace
        .subscribe(&String::from("/danube").try_into().unwrap())
        .await
        .unwrap();

    // Output resource
    let parana_resource: &str = "/parana";

    println!("Hamburg: Starting loop");
    loop {
        select!(
            change = tigris_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        println!("Hamburg: Received value from /tigris");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = ganges_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        println!("Hamburg: Received value from /ganges");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = nile_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        println!("Hamburg: Received value from /nile");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = danube_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let data = change.value.unwrap();
                        println!("Hamburg: Received value from /nile; putting it to {}", parana_resource);
                        workspace
                            .put(&parana_resource.try_into().unwrap(), data.into())
                            .await
                            .unwrap();
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
