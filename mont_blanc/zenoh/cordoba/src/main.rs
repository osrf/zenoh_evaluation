use async_std::task;
use rand::random;
use std::convert::TryInto;
use std::time::Duration;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let resource: &str = "/amazon";
    println!("Cordoba: Data generation started");
    let data: f64 = random::<f64>() * 1000000.0;
    println!("Cordoba: Data generation done");
    println!("Cordoba: Starting loop");
    loop {
        println!("Cordoba: Putting generated value to resource {}", resource);
        workspace
            .put(&resource.try_into().unwrap(), data.into())
            .await
            .unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
