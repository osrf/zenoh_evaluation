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

    let resource: &str = "/chenab";
    println!("Hebron: Data generation started");
    let data: data_types::Quaternion = random();
    println!("Hebron: Data generation done");
    println!("Hebron: Starting loop");
    loop {
        let buf = serialize_quaternion(&data);
        println!(
            "Hebron: Putting generated quaternion to resource {}",
            resource
        );
        let value = Value::Custom {
            encoding_descr: String::from("protobuf"),
            data: ZBuf::from(buf),
        };
        workspace
            .put(&resource.try_into().unwrap(), value)
            .await
            .unwrap();
        task::sleep(Duration::from_millis(100)).await;
    }
}
