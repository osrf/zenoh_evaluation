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

    let port_number = 7507 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let murray_resource = format!("/{}/murray", robot_number);
    let mut murray_subscriber = session.subscribe(&murray_resource).await.unwrap();
    let lena_resource = format!("/{}/lena", robot_number);
    let mut lena_subscriber = session.subscribe(&lena_resource).await.unwrap();

    // Output resources
    let volga_resource = format!("/{}/volga", robot_number);
    let volga_expression_id = session.declare_expr(&volga_resource).await.unwrap();
    session
        .declare_publication(volga_expression_id)
        .await
        .unwrap();

    let node_name = format!("Georgetown_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let data: f64 = random();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    let mut start_time = Instant::now();
    loop {
        select!(
            change = murray_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _vec3s = deserialize_vector3_stamped(buf.payload.contiguous().as_slice()).unwrap();
                        println!("{}: Received Vector3Stamped from {}", node_name, murray_resource);
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
                        println!("{}: Received WrenchStamped from {}", node_name, lena_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 50 {
                    start_time = Instant::now();

                    println!("{}: Putting generated Float64 to {}", node_name, volga_resource);
                    session.put(volga_expression_id, data).await.unwrap();
                }
            },
        )
    }
}
