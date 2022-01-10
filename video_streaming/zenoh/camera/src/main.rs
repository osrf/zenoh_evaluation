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

    let resource: &str = "/camera";
    println!("Camera: Data generation started");
    let mut image: data_types::Image = random();
    println!("Camera: Data generation done");
    loop {
        println!("Camera: Putting generated image to resource {}", resource);
        image.header = random();
        let buf = serialize_image(&image);
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
