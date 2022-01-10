use async_std::task;
use bigdata::*;
use std::convert::TryInto;
use std::time::Duration;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let mut datum = create_big_data();
    println!("Data is ready to transmit");
    loop {
        let resource: &str = "/amazon";
        set_big_data_timestamp_to_now(&mut datum);
        let buf = serialize_big_data(&datum);
        let data_size = buf.len();
        let value = Value::Custom {
            encoding_descr: String::from("protobuf"),
            data: ZBuf::from(buf),
        };
        println!(
            "Putting {} bytes of data with timestamp {:?} to resource {}",
            data_size,
            datum.timestamp.as_ref().unwrap(),
            resource
        );
        workspace
            .put(&resource.try_into().unwrap(), value)
            .await
            .unwrap();
        task::sleep(Duration::from_secs(1)).await;
    }
}
