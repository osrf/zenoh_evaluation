use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::env;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap().parse::<i16>().unwrap();

    let port_number = 7506 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let parana_resource = format!("/{}/parana", robot_number);
    let mut parana_subscriber = session.subscribe(&parana_resource).await.unwrap();
    let danube_resource = format!("/{}/danube", robot_number);
    let mut danube_subscriber = session.subscribe(&danube_resource).await.unwrap();
    let tagus_resource = format!("/{}/tagus", robot_number);
    let mut tagus_subscriber = session.subscribe(&tagus_resource).await.unwrap();
    let congo_resource = format!("/{}/congo", robot_number);
    let mut congo_subscriber = session.subscribe(&congo_resource).await.unwrap();

    // Output resources
    let arkansas_resource = format!("/{}/arkansas", robot_number);
    let arkansas_expression_id = session.declare_expr(&arkansas_resource).await.unwrap();
    session
        .declare_publication(arkansas_expression_id)
        .await
        .unwrap();

    let node_name = format!("Geneva_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let float_data: f32 = random::<f32>() * 1000.0;
    println!("{}: Data generation done", node_name);
    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = parana_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _string_data = change.value;
                        println!("{}: Received String value from {}; putting Float32 to {}", node_name, parana_resource, arkansas_resource);
                        session
                            .put(arkansas_expression_id, float_data as f64)
                            .await
                            .unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = danube_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("{}: Received value from {}", node_name, danube_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = tagus_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _pose = deserialize_pose(buf.payload.contiguous().as_slice()).unwrap();
                        println!("{}: Received Pose from {}", node_name, tagus_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = congo_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _twist = deserialize_twist(buf.payload.contiguous().as_slice()).unwrap();
                        println!("{}: Received Twist from {}", node_name, congo_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
