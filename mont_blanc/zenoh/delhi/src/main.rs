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

    let resource: &str = "/columbia";
    println!("Delhi: Data generation started");
    let data: data_types::Image = random();
    println!("Delhi: Data generation done");
    println!("Delhi: Starting loop");
    loop {
        let buf = serialize_image(&data);
        println!(
            "Delhi: Putting image with {} bytes to resource {}",
            buf.len(),
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
        task::sleep(Duration::from_secs(1)).await;
    }
}
