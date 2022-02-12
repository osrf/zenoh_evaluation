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

    let port_number = 7511 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let amazon_resource = format!("/{}/amazon", robot_number);
    let mut amazon_subscriber = session.subscribe(&amazon_resource).await.unwrap();

    let tigris_resource = format!("/{}/tigris", robot_number);
    let tigris_expression_id = session.declare_expr(&tigris_resource).await.unwrap();
    session
        .declare_publication(tigris_expression_id)
        .await
        .unwrap();

    let node_name = format!("Lyon_{}", robot_number);
    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = amazon_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let data = change.value;
                        println!("{}: Received value, putting it to resource {}", node_name, tigris_resource);
                        session.put(tigris_expression_id, data).await.unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
