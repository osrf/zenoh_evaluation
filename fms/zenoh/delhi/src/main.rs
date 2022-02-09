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
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7504"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let columbia_resource_path = format!("/{}/columbia", robot_number);
    println!("Delhi: Data generation started");
    let data: data_types::Image = random();
    println!("Delhi: Data generation done");
    println!("Delhi: Starting loop");
    loop {
        let buf = serialize_image(&data);
        println!(
            "Delhi: Putting image with {} bytes to resource {}",
            buf.len(),
            columbia_resource_path
        );
        let value = Value::Custom {
            encoding_descr: String::from("protobuf"),
            data: ZBuf::from(buf),
        };
        workspace
            .put(&columbia_resource_path.clone().try_into().unwrap(), value)
            .await
            .unwrap();
        task::sleep(Duration::from_secs(1)).await;
    }
}
