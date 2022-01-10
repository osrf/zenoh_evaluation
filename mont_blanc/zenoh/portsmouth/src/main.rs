use async_std::task;
use datatypes::random_string;
use std::convert::TryInto;
use std::time::Duration;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let resource: &str = "/danube";
    println!("Portsmouth: Data generation started");
    let data = random_string(256);
    println!("Portsmouth: Data generation done");
    println!("Portsmouth: Starting loop");
    loop {
        println!(
            "Portsmouth: Putting generated value to resource {}",
            resource
        );
        workspace
            .put(&resource.try_into().unwrap(), data.clone().into())
            .await
            .unwrap();
        task::sleep(Duration::from_millis(200)).await;
    }
}
