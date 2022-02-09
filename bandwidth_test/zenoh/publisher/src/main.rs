use async_std::task;
use bigdata::*;
use std::time::{Duration, Instant};
use zenoh;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7501".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();
    let expression_id = session.declare_expr("/amazon").await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    let mut data = create_big_data();
    println!("Data is ready to transmit");
    loop {
        let resource: &str = "/amazon";
        set_big_data_timestamp_to_now(&mut data);
        let mut start_instant = Instant::now();
        let buf = serialize_big_data(&data);
        let data_size = buf.len();
        println!(
            "Serialisation took {}",
            start_instant.elapsed().as_secs_f64()
        );
        println!(
            "Putting {} bytes of data with timestamp {:?} to resource {}",
            data_size,
            data.timestamp.as_ref().unwrap(),
            resource
        );
        start_instant = Instant::now();
        session.put(expression_id, buf).await.unwrap();
        println!(
            "Transmission took {}",
            start_instant.elapsed().as_secs_f64()
        );
        task::sleep(Duration::from_secs(1)).await;
    }
}
