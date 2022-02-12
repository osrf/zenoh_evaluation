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

    let port_number = 7518 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Inputs
    let mekong_resource = format!("/{}/mekong", robot_number);
    let mut mekong_subscriber = session.subscribe(&mekong_resource).await.unwrap();

    // Outpus
    let murray_resource = format!("/{}/murray", robot_number);
    let murray_expression_id = session.declare_expr(&murray_resource).await.unwrap();
    session
        .declare_publication(murray_expression_id)
        .await
        .unwrap();

    let node_name = format!("Rotterdam_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let header_data: data_types::Header = random();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = mekong_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let twist_with_cov = deserialize_twist_with_covariance_stamped(buf.contiguous().as_slice()).unwrap();
                        let vec3s = data_types::Vector3Stamped {
                            header: Some(header_data.clone()),
                            vector: twist_with_cov.twist.unwrap().twist.unwrap().linear,
                        };
                        println!(
                            "{}: Received TwistWithCovariance from {}, putting Vector3Stamped to {}",
                            node_name,
                            mekong_resource,
                            murray_resource);
                        let vec3s_buf = serialize_vector3_stamped(&vec3s);
                        session
                            .put(murray_expression_id, vec3s_buf)
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
