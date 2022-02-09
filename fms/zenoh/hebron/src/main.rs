use async_std::task;
use datatypes::*;
use rand::random;
use std::convert::TryInto;
use std::env;
use std::time::Duration;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7509"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let chenab_resource_path = format!("/{}/chenab", robot_number);
    println!("Hebron: Data generation started");
    let data: data_types::Quaternion = random();
    println!("Hebron: Data generation done");
    println!("Hebron: Starting loop");
    loop {
        let buf = serialize_quaternion(&data);
        println!(
            "Hebron: Putting generated quaternion to resource {}",
            chenab_resource_path
        );
        let value = Value::Custom {
            encoding_descr: String::from("protobuf"),
            data: ZBuf::from(buf),
        };
        workspace
            .put(&chenab_resource_path.clone().try_into().unwrap(), value)
            .await
            .unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
