use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use std::time::Instant;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let mut murray_change_stream = workspace
        .subscribe(&String::from("/murray").try_into().unwrap())
        .await
        .unwrap();
    let mut lena_change_stream = workspace
        .subscribe(&String::from("/lena").try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let volga_resource: &str = "/volga";
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
                        println!("Georgetown: Received Vector3Stamped from /murray");
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
                        println!("Georgetown: Received WrenchStamped from /lena");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 50 {
                    start_time = Instant::now();

                    println!("Georgetown: Putting generated Float64 to {}", volga_resource);
                    workspace
                        .put(&volga_resource.try_into().unwrap(), data.into())
                        .await
                        .unwrap();
                }
            },
        )
    }
}
