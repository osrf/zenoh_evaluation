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
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7508"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let tigris_resource_path = format!("/{}/tigris", robot_number);
    let mut tigris_change_stream = workspace
        .subscribe(&tigris_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let ganges_resource_path = format!("/{}/ganges", robot_number);
    let mut ganges_change_stream = workspace
        .subscribe(&ganges_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let nile_resource_path = format!("/{}/nile", robot_number);
    let mut nile_change_stream = workspace
        .subscribe(&nile_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let danube_resource_path = format!("/{}/danube", robot_number);
    let mut danube_change_stream = workspace
        .subscribe(&danube_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Output resource
    let parana_resource_path = format!("/{}/parana", robot_number);

    println!("Hamburg: Starting loop");
    loop {
        select!(
            change = tigris_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        println!("Hamburg: Received value from {}", tigris_resource_path);
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
                        println!("Hamburg: Received value from {}", ganges_resource_path);
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
                        println!("Hamburg: Received value from {}", nile_resource_path);
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
                        println!("Hamburg: Received value from {}; putting it to {}", nile_resource_path, parana_resource_path);
                        workspace
                            .put(&parana_resource_path.clone().try_into().unwrap(), data.into())
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
