use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let mut parana_change_stream = workspace
        .subscribe(&String::from("/parana").try_into().unwrap())
        .await
        .unwrap();
    let mut danube_change_stream = workspace
        .subscribe(&String::from("/danube").try_into().unwrap())
        .await
        .unwrap();
    let mut tagus_change_stream = workspace
        .subscribe(&String::from("/tagus").try_into().unwrap())
        .await
        .unwrap();
    let mut congo_change_stream = workspace
        .subscribe(&String::from("/congo").try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let arkansas_resource: &str = "/arkansas";

    println!("Geneva: Data generation started");
    let float_data: f32 = random::<f32>() * 1000.0;
    println!("Geneva: Data generation done");
    println!("Geneva: Starting loop");
    loop {
        select!(
            change = parana_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _string_data = change.value.unwrap();
                        println!("Geneva: Received String value from /parana; putting Float32 to {}", arkansas_resource);
                        workspace
                            .put(&arkansas_resource.try_into().unwrap(), (float_data as f64).into())
                            .await
                            .unwrap();
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = danube_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Geneva: Received value from /danube");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = tagus_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _pose = deserialize_pose(buf.contiguous().as_slice()).unwrap();
                        println!("Geneva: Received Pose from /tagus");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = congo_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _twist = deserialize_twist(buf.contiguous().as_slice()).unwrap();
                        println!("Geneva: Received Twist from /congo");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
