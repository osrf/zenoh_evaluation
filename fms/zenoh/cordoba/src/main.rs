use async_std::task;
use rand::random;
use std::convert::TryInto;
use std::env;
use std::time::Duration;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let amazon_resource_path = format!("/{}/amazon", robot_number);
    println!("Cordoba: Data generation started");
    let data: f64 = random::<f64>() * 1000000.0;
    println!("Cordoba: Data generation done");
    println!("Cordoba: Starting loop");
    loop {
        println!(
            "Cordoba: Putting generated value to resource {}",
            amazon_resource_path
        );
        workspace
            .put(
                &amazon_resource_path.clone().try_into().unwrap(),
                data.into(),
            )
            .await
            .unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
