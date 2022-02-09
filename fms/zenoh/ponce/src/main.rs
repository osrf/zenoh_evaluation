use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use std::env;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap();

    env_logger::init();

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7516"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let tagus_resource_path = format!("/{}/tagus", robot_number);
    let mut tagus_change_stream = workspace
        .subscribe(&tagus_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let danube_resource_path = format!("/{}/danube", robot_number);
    let mut danube_change_stream = workspace
        .subscribe(&danube_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let missouri_resource_path = format!("/{}/missouri", robot_number);
    let mut missouri_change_stream = workspace
        .subscribe(&missouri_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let brazos_resource_path = format!("/{}/brazos", robot_number);
    let mut brazos_change_stream = workspace
        .subscribe(&brazos_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let yamuna_resource_path = format!("/{}/yamuna", robot_number);
    let mut yamuna_change_stream = workspace
        .subscribe(&yamuna_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let godavari_resource_path = format!("/{}/godavari", robot_number);
    let mut godavari_change_stream = workspace
        .subscribe(&godavari_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let loire_resource_path = format!("/{}/loire", robot_number);
    let mut loire_change_stream = workspace
        .subscribe(&loire_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let ohio_resource_path = format!("/{}/ohio", robot_number);
    let mut ohio_change_stream = workspace
        .subscribe(&ohio_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let volga_resource_path = format!("/{}/volga", robot_number);
    let mut volga_change_stream = workspace
        .subscribe(&volga_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let congo_resource_path = format!("/{}/congo", robot_number);
    let mekong_resource_path = format!("/{}/mekong", robot_number);

    println!("Ponce: Data generation started");
    let twist_data: data_types::Twist = random();
    let twist_with_cov_data: data_types::TwistWithCovarianceStamped = random();
    println!("Ponce: Data generation done");
    println!("Ponce: Starting loop");
    loop {
        select!(
            change = tagus_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _pose = deserialize_pose(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received Pose from {}", tagus_resource_path);
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
                        println!("Ponce: Received value from {}", danube_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = missouri_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received Image of {} bytes from {}", image_size, missouri_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = brazos_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let pc_size = buf.len();
                        let _pc = deserialize_pointcloud2(buf.contiguous().as_slice()).unwrap();

                        let twist_buf = serialize_twist(&twist_data);
                        let twist_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(twist_buf),
                        };

                        let twist_with_cov_buf = serialize_twist_with_covariance_stamped(&twist_with_cov_data);
                        let twist_with_cov_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(twist_with_cov_buf),
                        };

                        println!("Ponce: Received PointCloud2 of {} bytes from {}, Putting Twist to {} and TwistWithCovariance to {}",
                            pc_size, brazos_resource_path, congo_resource_path,
                            mekong_resource_path);
                        workspace
                            .put(&congo_resource_path.clone().try_into().unwrap(), twist_value)
                            .await
                            .unwrap();
                        workspace
                            .put(&mekong_resource_path.clone().try_into().unwrap(), twist_with_cov_value)
                            .await
                            .unwrap();
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = yamuna_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _vec3 = deserialize_vector3(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received Vector3 from {}", yamuna_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = godavari_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let scan_size = buf.len();
                        let _scan = deserialize_laserscan(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received LaserScan of {} bytes from {}", scan_size, godavari_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = loire_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let pc_size = buf.len();
                        let _pc = deserialize_pointcloud2(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received PointCloud2 of {} bytes from {}", pc_size, loire_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = ohio_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Ponce: Received value from {}", ohio_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = volga_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Ponce: Received value from {}", volga_resource_path);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
