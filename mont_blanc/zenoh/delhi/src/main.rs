use async_std::task;
use datatypes::*;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7504".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource: &str = "/columbia";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Delhi: Data generation started");
    let data: data_types::Image = random();
    println!("Delhi: Data generation done");
    println!("Delhi: Starting loop");
    loop {
        let buf = serialize_image(&data);
        println!(
            "Delhi: Putting image with {} bytes to resource {}",
            buf.len(),
            resource
        );
        session.put(expression_id, buf).await.unwrap();
        task::sleep(Duration::from_secs(1)).await;
    }
}
