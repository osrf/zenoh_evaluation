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

    let resource: &str = "/nile";
    println!("Medellin: Data generation started");
    let data = random::<i32>();
    println!("Medellin: Data generation done");
    println!("Medellin: Starting loop");
    loop {
        println!("Medellin: Putting generated value to resource {}", resource);
        workspace
            .put(&resource.try_into().unwrap(), (data as i64).into())
            .await
            .unwrap();
        task::sleep(Duration::from_millis(10)).await;
    }
}
