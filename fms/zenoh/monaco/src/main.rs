use datatypes::*;
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

    let port_number = 7514 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Inputs
    let congo_resource = format!("/{}/congo", robot_number);
    let mut congo_subscriber = session.subscribe(&congo_resource).await.unwrap();

    // Outpus
    let ohio_resource = format!("/{}/ohio", robot_number);
    let ohio_expression_id = session.declare_expr(&ohio_resource).await.unwrap();
    session
        .declare_publication(ohio_expression_id)
        .await
        .unwrap();

    let node_name = format!("Monaco_{}", robot_number);
    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = congo_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let twist = deserialize_twist(buf.payload.contiguous().as_slice()).unwrap();
                        println!("{}: Received Twist from {}, putting value to {}", node_name, congo_resource, ohio_resource);
                        session
                            .put(ohio_expression_id, twist.linear.unwrap().x)
                            .await
                            .unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
