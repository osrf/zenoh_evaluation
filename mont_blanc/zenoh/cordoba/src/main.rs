use async_std::task;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7503".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/amazon";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Cordoba: Data generation started");
    let data: f64 = random::<f64>() * 1000000.0;
    println!("Cordoba: Data generation done");
    println!("Cordoba: Starting loop");
    loop {
        println!("Cordoba: Putting generated value to resource {}", resource);
        session.put(expression_id, data).await.unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
