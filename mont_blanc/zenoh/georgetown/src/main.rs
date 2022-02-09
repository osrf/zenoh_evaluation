use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::time::Instant;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7507".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let murray_resource = "/murray";
    let mut murray_subscriber = session.subscribe(murray_resource).await.unwrap();
    let lena_resource = "/lena";
    let mut lena_subscriber = session.subscribe(lena_resource).await.unwrap();

    // Output resources
    let volga_resource: &str = "/volga";
    let volga_expression_id = session.declare_expr(volga_resource).await.unwrap();
    session
        .declare_publication(volga_expression_id)
        .await
        .unwrap();

    println!("Georgetown: Data generation started");
    let data: f64 = random();
    println!("Georgetown: Data generation done");

    println!("Georgetown: Starting loop");
    let mut start_time = Instant::now();
    loop {
        select!(
            change = murray_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _vec3s = deserialize_vector3_stamped(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Georgetown: Received Vector3Stamped from {}", murray_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = lena_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _wrench = deserialize_wrench_stamped(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Georgetown: Received WrenchStamped from {}", lena_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 50 {
                    start_time = Instant::now();

                    println!("Georgetown: Putting generated Float64 to {}", volga_resource);
                    session.put(volga_expression_id, data).await.unwrap();
                }
            },
        )
    }
}
