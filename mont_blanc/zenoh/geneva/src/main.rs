use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7506".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let parana_resource = "/parana";
    let mut parana_subscriber = session.subscribe(parana_resource).await.unwrap();
    let danube_resource = "/danube";
    let mut danube_subscriber = session.subscribe(danube_resource).await.unwrap();
    let tagus_resource = "/tagus";
    let mut tagus_subscriber = session.subscribe(tagus_resource).await.unwrap();
    let congo_resource = "/congo";
    let mut congo_subscriber = session.subscribe(congo_resource).await.unwrap();

    // Output resources
    let arkansas_resource: &str = "/arkansas";
    let arkansas_expression_id = session.declare_expr(arkansas_resource).await.unwrap();
    session
        .declare_publication(arkansas_expression_id)
        .await
        .unwrap();

    println!("Geneva: Data generation started");
    let float_data: f32 = random::<f32>() * 1000.0;
    println!("Geneva: Data generation done");
    println!("Geneva: Starting loop");
    loop {
        select!(
            change = parana_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _string_data = change.value;
                        println!("Geneva: Received String value from {}; putting Float32 to {}", parana_resource, arkansas_resource);
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
                        println!("Geneva: Received value from {}", danube_resource);
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
                        println!("Geneva: Received Pose from {}", tagus_resource);
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
                        println!("Geneva: Received Twist from {}", congo_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
