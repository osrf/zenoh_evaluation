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

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7513"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let nile_resource_path = format!("/{}/nile", robot_number);
    println!("Medellin: Data generation started");
    let data = random::<i32>();
    println!("Medellin: Data generation done");
    println!("Medellin: Starting loop");
    loop {
        println!(
            "Medellin: Putting generated value to resource {}",
            nile_resource_path
        );
        workspace
            .put(
                &nile_resource_path.clone().try_into().unwrap(),
                (data as i64).into(),
            )
            .await
            .unwrap();
        task::sleep(Duration::from_millis(10)).await;
    }
}
