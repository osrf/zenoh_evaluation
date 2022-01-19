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

    let ganges_resource_path = format!("/{}/ganges", robot_number);
    println!("Freeport: Data generation started");
    let data: i64 = random::<i64>();
    println!("Freeport: Data generation done");
    println!("Freeport: Starting loop");
    loop {
        println!(
            "Freeport: Putting generated value to resource {}",
            ganges_resource_path
        );
        workspace
            .put(
                &ganges_resource_path.clone().try_into().unwrap(),
                data.into(),
            )
            .await
            .unwrap();
        task::sleep(Duration::from_millis(50)).await;
    }
}
