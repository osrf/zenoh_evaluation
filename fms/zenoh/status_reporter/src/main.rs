use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use std::env;
use std::time::Instant;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let arkansas_resource_path = format!("/{}/arkansas", robot_number);
    let mut change_stream = workspace
        .subscribe(&arkansas_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    let status_resource: &str = "/status";
    println!("Status reporter: Data generation started");
    let mut status: data_types::RobotStatus = random();
    println!("Status reporter: Data generation done");

    let mut start_time = Instant::now();
    loop {
        select!(
            change = change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Status reporter: Received value from {}", arkansas_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 1000 {
                    start_time = Instant::now();
                    println!("Status reporter: Putting generated status to resource {}", status_resource);
                    status.header = random();
                    let buf = serialize_robot_status(&status);
                    let value = Value::Custom {
                        encoding_descr: String::from("protobuf"),
                        data: ZBuf::from(buf),
                    };
                    workspace
                        .put(&status_resource.try_into().unwrap(), value)
                        .await
                        .unwrap();
                }
            }
        )
    }
}
