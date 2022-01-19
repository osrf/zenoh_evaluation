use async_std::task;
use datatypes::random_string;
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

    let danube_resource_path = format!("/{}/danube", robot_number);
    println!("Portsmouth: Data generation started");
    let data = random_string(256);
    println!("Portsmouth: Data generation done");
    println!("Portsmouth: Starting loop");
    loop {
        println!(
            "Portsmouth: Putting generated value to resource {}",
            danube_resource_path
        );
        workspace
            .put(
                &danube_resource_path.clone().try_into().unwrap(),
                data.clone().into(),
            )
            .await
            .unwrap();
        task::sleep(Duration::from_millis(200)).await;
    }
}
