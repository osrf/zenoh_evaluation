use futures::prelude::*;
use futures::select;
use std::env;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap().parse::<i16>().unwrap();

    let port_number = 7508 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let tigris_resource = format!("/{}/tigris", robot_number);
    let mut tigris_subscriber = session.subscribe(&tigris_resource).await.unwrap();
    let ganges_resource = format!("/{}/ganges", robot_number);
    let mut ganges_subscriber = session.subscribe(&ganges_resource).await.unwrap();
    let nile_resource = format!("/{}/nile", robot_number);
    let mut nile_subscriber = session.subscribe(&nile_resource).await.unwrap();
    let danube_resource = format!("/{}/danube", robot_number);
    let mut danube_subscriber = session.subscribe(&danube_resource).await.unwrap();

    // Output resource
    let parana_resource = format!("/{}/parana", robot_number);
    let parana_expression_id = session.declare_expr(&parana_resource).await.unwrap();
    session
        .declare_publication(parana_expression_id)
        .await
        .unwrap();

    let node_name = format!("Hamburg_{}", robot_number);
    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = tigris_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        println!("{}: Received value from {}", node_name, tigris_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = ganges_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        println!("{}: Received value from {}", node_name, ganges_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = nile_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        println!("{}: Received value from {}", node_name, nile_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = danube_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let data = change.value;
                        println!("{}: Received value from {}; putting it to {}", node_name, nile_resource, parana_resource);
                        session.put(parana_expression_id, data).await.unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
