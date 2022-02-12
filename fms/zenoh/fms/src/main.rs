use datatypes::*;
use futures::prelude::*;
use futures::select;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7500".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let status_resource = "/status";
    let mut status_subscriber = session.subscribe(status_resource).await.unwrap();

    println!("FMS: Starting loop");
    loop {
        select!(
            change = status_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let status_size = buf.len();
                        let _status = deserialize_robot_status(buf.contiguous().as_slice()).unwrap();
                        println!("FMS: Received status of {} bytes from /status", status_size);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
