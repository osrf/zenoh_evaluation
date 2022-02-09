use async_std::task;
use datatypes::*;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7510".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/yamuna";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Kingston: Data generation started");
    let data: data_types::Vector3 = random();
    println!("Kingston: Data generation done");

    println!("Kingston: Starting loop");
    loop {
        let buf = serialize_vector3(&data);
        println!(
            "Kingston: Putting generated Vector3 to resource {}",
            resource
        );
        session.put(expression_id, buf).await.unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
