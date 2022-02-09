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
    config.listeners.push("tcp/0.0.0.0:7518".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Inputs
    let mekong_resource = "/mekong";
    let mut mekong_subscriber = session.subscribe(mekong_resource).await.unwrap();

    // Outpus
    let murray_resource: &str = "/murray";
    let murray_expression_id = session.declare_expr(murray_resource).await.unwrap();
    session
        .declare_publication(murray_expression_id)
        .await
        .unwrap();

    println!("Rotterdam: Data generation started");
    let header_data: data_types::Header = random();
    println!("Rotterdam: Data generation done");

    println!("Rotterdam: Starting loop");
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
                            "Rotterdam: Received TwistWithCovariance from {}, putting Vector3Stamped to {}",
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
