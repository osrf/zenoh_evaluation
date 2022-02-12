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

    let port_number = 7502 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Inputs
    let mekong_resource = format!("/{}/mekong", robot_number);
    let mut mekong_subscriber = session.subscribe(mekong_resource).await.unwrap();

    // Outputs
    let lena_resource: &str = "/lena";
    let expression_id = session.declare_expr(lena_resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    let node_name = format!("Barcelona_{}", robot_number);
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
                        let buf = change.value;
                        let twist_with_cov = deserialize_twist_with_covariance_stamped(buf.payload.contiguous().as_slice()).unwrap();
                        let twist = twist_with_cov.twist.unwrap().twist.unwrap();
                        let wrench = data_types::WrenchStamped {
                            header: Some(header_data.clone()),
                            wrench: Some(data_types::Wrench {
                                force: twist.linear,
                                torque: twist.angular,
                            }),
                        };
                        println!("{}: Received TwistWithCovariance from /mekong, putting WrenchStamped to {}", node_name, lena_resource);
                        let wrench_buf = serialize_wrench_stamped(&wrench);
                        session.put(expression_id, wrench_buf).await.unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
