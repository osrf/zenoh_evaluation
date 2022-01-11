use async_std::task;
use bigdata::*;
use std::convert::TryInto;
use std::time::{Duration, Instant};
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();

    let workspace = zenoh.workspace(None).await.unwrap();

    let mut data = create_big_data();
    println!("Data is ready to transmit");
    loop {
        let resource: &str = "/amazon";
        set_big_data_timestamp_to_now(&mut data);
        let mut start_instant = Instant::now();
        let buf = serialize_big_data(&data);
        println!(
            "Serialisation took {}",
            start_instant.elapsed().as_secs_f64()
        );
        start_instant = Instant::now();
        let data_size = buf.len();
        let value = Value::Custom {
            encoding_descr: String::from("protobuf"),
            data: ZBuf::from(buf),
        };
        println!(
            "Buffer preparation took {}",
            start_instant.elapsed().as_secs_f64()
        );
        println!(
            "Putting {} bytes of data with timestamp {:?} to resource {}",
            data_size,
            data.timestamp.as_ref().unwrap(),
            resource
        );
        start_instant = Instant::now();
        workspace
            .put(&resource.try_into().unwrap(), value)
            .await
            .unwrap();
        println!(
            "Transmission took {}",
            start_instant.elapsed().as_secs_f64()
        );
        task::sleep(Duration::from_secs(1)).await;
    }
}
