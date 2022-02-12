use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::env;
use std::time::Instant;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap().parse::<i16>().unwrap();

    let port_number = 7521 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let arkansas_resource = format!("/{}/arkansas", robot_number);
    let mut arkansas_subscriber = session.subscribe(&arkansas_resource).await.unwrap();

    let status_resource: String = String::from("/status");
    let status_expression_id = session.declare_expr(&status_resource).await.unwrap();
    session
        .declare_publication(status_expression_id)
        .await
        .unwrap();

    let node_name = format!("Status_reporter_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let mut status: data_types::RobotStatus = random();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    let mut start_time = Instant::now();
    loop {
        select!(
            change = arkansas_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("{}: Received value from {}", node_name, arkansas_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 1000 {
                    start_time = Instant::now();
                    println!("{}: Putting generated status to resource {}", node_name, status_resource);
                    status.header = random();
                    let buf = serialize_robot_status(&status);
                    session.put(status_expression_id, buf).await.unwrap();
                }
            }
        )
    }
}
