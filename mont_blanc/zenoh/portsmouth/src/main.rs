use async_std::task;
use datatypes::random_string;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7517".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/danube";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Portsmouth: Data generation started");
    let data = random_string(256);
    println!("Portsmouth: Data generation done");

    println!("Portsmouth: Starting loop");
    loop {
        println!(
            "Portsmouth: Putting generated value to resource {}",
            resource
        );
        session.put(expression_id, data.clone()).await.unwrap();
        task::sleep(Duration::from_millis(200)).await;
    }
}
