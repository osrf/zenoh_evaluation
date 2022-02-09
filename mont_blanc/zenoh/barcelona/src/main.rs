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
    config.listeners.push("tcp/0.0.0.0:7502".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Inputs
    let mekong_resource: &str = "/mekong";
    let mut mekong_subscriber = session.subscribe(mekong_resource).await.unwrap();

    // Outputs
    let lena_resource: &str = "/lena";
    let expression_id = session.declare_expr(lena_resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Barcelona: Data generation started");
    let header_data: data_types::Header = random();
    println!("Barcelona: Data generation done");
    println!("Barcelona: Starting loop");
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
                        println!("Barcelona: Received TwistWithCovariance from /mekong, putting WrenchStamped to {}", lena_resource);
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
