use async_std::task;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7513".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/nile";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Medellin: Data generation started");
    let data = random::<i32>();
    println!("Medellin: Data generation done");

    println!("Medellin: Starting loop");
    loop {
        println!("Medellin: Putting generated value to resource {}", resource);
        session.put(expression_id, data as i64).await.unwrap();
        task::sleep(Duration::from_millis(10)).await;
    }
}
