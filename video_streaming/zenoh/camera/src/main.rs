use async_std::task;
use datatypes::*;
use rand::random;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7503".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource = "/camera";
    let expression_id = session.declare_expr(resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Camera: Data generation started");
    let mut image: data_types::Image = random();
    println!("Camera: Data generation done");

    println!("Camera: Starting loop");
    loop {
        println!("Camera: Putting generated image to resource {}", resource);
        image.header = random();
        let buf = serialize_image(&image);
        session.put(expression_id, buf).await.unwrap();
        task::sleep(Duration::from_secs(1)).await;
    }
}
