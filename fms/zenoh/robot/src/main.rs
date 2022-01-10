use async_std::task;
use datatypes::*;
use rand::random;
use std::convert::TryInto;
use std::time::Duration;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let resource: &str = "/status";
    println!("Robot: Data generation started");
    let mut status: data_types::RobotStatus = random();
    println!("Robot: Data generation done");
    loop {
        println!("Robot: Putting generated status to resource {}", resource);
        status.header = random();
        let buf = serialize_robot_status(&status);
        let value = Value::Custom {
            encoding_descr: String::from("protobuf"),
            data: ZBuf::from(buf),
        };
        workspace
            .put(&resource.try_into().unwrap(), value)
            .await
            .unwrap();
        task::sleep(Duration::from_secs(1)).await;
    }
}
