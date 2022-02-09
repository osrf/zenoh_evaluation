use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use std::env;
use std::time::Instant;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7507"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let murray_resource_path = format!("/{}/murray", robot_number);
    let mut murray_change_stream = workspace
        .subscribe(&murray_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let lena_resource_path = format!("/{}/lena", robot_number);
    let mut lena_change_stream = workspace
        .subscribe(&lena_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let volga_resource_path = format!("/{}/volga", robot_number);
    println!("Georgetown: Data generation started");
    let data: f64 = random();
    println!("Georgetown: Data generation done");

    println!("Georgetown: Starting loop");
    let mut start_time = Instant::now();
    loop {
        select!(
            change = murray_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _vec3s = deserialize_vector3_stamped(buf.contiguous().as_slice()).unwrap();
                        println!("Georgetown: Received Vector3Stamped from {}", murray_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = lena_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _wrench = deserialize_wrench_stamped(buf.contiguous().as_slice()).unwrap();
                        println!("Georgetown: Received WrenchStamped from {}", lena_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 50 {
                    start_time = Instant::now();

                    println!("Georgetown: Putting generated Float64 to {}", volga_resource_path);
                    workspace
                        .put(&volga_resource_path.clone().try_into().unwrap(), data.into())
                        .await
                        .unwrap();
                }
            },
        )
    }
}
