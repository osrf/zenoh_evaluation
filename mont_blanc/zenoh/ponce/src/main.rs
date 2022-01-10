use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let mut tagus_change_stream = workspace
        .subscribe(&String::from("/tagus").try_into().unwrap())
        .await
        .unwrap();
    let mut danube_change_stream = workspace
        .subscribe(&String::from("/danube").try_into().unwrap())
        .await
        .unwrap();
    let mut missouri_change_stream = workspace
        .subscribe(&String::from("/missouri").try_into().unwrap())
        .await
        .unwrap();
    let mut brazos_change_stream = workspace
        .subscribe(&String::from("/brazos").try_into().unwrap())
        .await
        .unwrap();
    let mut yamuna_change_stream = workspace
        .subscribe(&String::from("/yamuna").try_into().unwrap())
        .await
        .unwrap();
    let mut godavari_change_stream = workspace
        .subscribe(&String::from("/godavari").try_into().unwrap())
        .await
        .unwrap();
    let mut loire_change_stream = workspace
        .subscribe(&String::from("/loire").try_into().unwrap())
        .await
        .unwrap();
    let mut ohio_change_stream = workspace
        .subscribe(&String::from("/ohio").try_into().unwrap())
        .await
        .unwrap();
    let mut volga_change_stream = workspace
        .subscribe(&String::from("/volga").try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let congo_resource: &str = "/congo";
    let mekong_resource: &str = "/mekong";

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
                        println!("Ponce: Received Pose from /tagus");
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
                        println!("Ponce: Received value from /danube");
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
                        println!("Ponce: Received Image of {} bytes from /missouri", image_size);
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

                        println!("Ponce: Received PointCloud2 of {} bytes from /brazos, Putting Twist to {} and TwistWithCovariance to {}",
                            pc_size, congo_resource,
                            mekong_resource);
                        workspace
                            .put(&congo_resource.try_into().unwrap(), twist_value)
                            .await
                            .unwrap();
                        workspace
                            .put(&mekong_resource.try_into().unwrap(), twist_with_cov_value)
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
                        println!("Ponce: Received Vector3 from /yamuna");
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
                        println!("Ponce: Received LaserScan of {} bytes from /godavari", scan_size);
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
                        println!("Ponce: Received PointCloud2 of {} bytes from /loire", pc_size);
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
                        println!("Ponce: Received value from /ohio");
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
                        println!("Ponce: Received value from /volga");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
