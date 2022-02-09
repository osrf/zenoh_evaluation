use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use std::env;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7506"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let parana_resource_path = format!("/{}/parana", robot_number);
    let mut parana_change_stream = workspace
        .subscribe(&parana_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let danube_resource_path = format!("/{}/danube", robot_number);
    let mut danube_change_stream = workspace
        .subscribe(&danube_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let tagus_resource_path = format!("/{}/tagus", robot_number);
    let mut tagus_change_stream = workspace
        .subscribe(&tagus_resource_path.clone().clone().try_into().unwrap())
        .await
        .unwrap();
    let congo_resource_path = format!("/{}/congo", robot_number);
    let mut congo_change_stream = workspace
        .subscribe(&congo_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let arkansas_resource_path = format!("/{}/arkansas", robot_number);

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
                        println!("Geneva: Received String value from {}; putting Float32 to {}", parana_resource_path, arkansas_resource_path);
                        workspace
                            .put(&arkansas_resource_path.clone().try_into().unwrap(), (float_data as f64).into())
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
                        println!("Geneva: Received value from {}", danube_resource_path);
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
                        println!("Geneva: Received Pose from {}", tagus_resource_path);
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
                        println!("Geneva: Received Twist from {}", congo_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
