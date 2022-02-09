use async_std::task;
use datatypes::*;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7509".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/chenab";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Hebron: Data generation started");
    let data: data_types::Quaternion = random();
    println!("Hebron: Data generation done");

    println!("Hebron: Starting loop");
    loop {
        let buf = serialize_quaternion(&data);
        println!(
            "Hebron: Putting generated quaternion to resource {}",
            resource
        );
        session.put(expression_id, buf).await.unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
