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

    let resource: &str = "/ganges";
    println!("Freeport: Data generation started");
    let data: i64 = random::<i64>();
    println!("Freeport: Data generation done");
    println!("Freeport: Starting loop");
    loop {
        println!("Freeport: Putting generated value to resource {}", resource);
        workspace
            .put(&resource.try_into().unwrap(), data.into())
            .await
            .unwrap();
        task::sleep(Duration::from_millis(50)).await;
    }
}
