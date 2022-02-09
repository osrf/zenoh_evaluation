use async_std::task;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7505".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/ganges";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Freeport: Data generation started");
    let data: i64 = random::<i64>();
    println!("Freeport: Data generation done");
    println!("Freeport: Starting loop");
    loop {
        println!("Freeport: Putting generated value to resource {}", resource);
        session.put(expression_id, data).await.unwrap();
        task::sleep(Duration::from_millis(50)).await;
    }
}
