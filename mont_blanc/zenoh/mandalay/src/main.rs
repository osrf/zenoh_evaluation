use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::convert::TryInto;
use std::time::Instant;
use zenoh::net::ZBuf;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let mut danube_change_stream = workspace
        .subscribe(&String::from("/danube").try_into().unwrap())
        .await
        .unwrap();
    let mut chenab_change_stream = workspace
        .subscribe(&String::from("/chenab").try_into().unwrap())
        .await
        .unwrap();
    let mut salween_change_stream = workspace
        .subscribe(&String::from("/salween").try_into().unwrap())
        .await
        .unwrap();
    let mut godavari_change_stream = workspace
        .subscribe(&String::from("/godavari").try_into().unwrap())
        .await
        .unwrap();
    let mut yamuna_change_stream = workspace
        .subscribe(&String::from("/yamuna").try_into().unwrap())
        .await
        .unwrap();
    let mut loire_change_stream = workspace
        .subscribe(&String::from("/loire").try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let tagus_resource: &str = "/tagus";
    let missouri_resource: &str = "/missouri";
    let brazos_resource: &str = "/brazos";

    println!("Mandalay: Data generation started");
    let pose_data: data_types::Pose = random();
    let image_data: data_types::Image = random();
    let pointcloud2_data: data_types::PointCloud2 = random();
    println!("Mandalay: Data generation done");

    println!("Mandalay: Starting loop");
    let mut start_time = Instant::now();
    loop {
        select!(
            change = danube_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Mandalay: Received value from /danube");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = chenab_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let _quat = deserialize_quaternion(buf.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received value from /chenab");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = salween_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let pc_size = buf.len();
                        let _pc = deserialize_pointcloud2(buf.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received PointCloud2 of {} bytes from /salween", pc_size);
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
                        println!("Mandalay: Received image of {} bytes from /godavari", scan_size);
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
                        println!("Mandalay: Received Vector3 from /yamuna");
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
                        println!("Mandalay: Received PointCloud2 of {} bytes from /loire", pc_size);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 100 {
                    start_time = Instant::now();

                    let pose_buf = serialize_pose(&pose_data);
                    let pose_value = Value::Custom {
                        encoding_descr: String::from("protobuf"),
                        data: ZBuf::from(pose_buf),
                    };

                    let image_buf = serialize_image(&image_data);
                    let image_buf_len = image_buf.len();
                    let image_value = Value::Custom {
                        encoding_descr: String::from("protobuf"),
                        data: ZBuf::from(image_buf),
                    };

                    let pointcloud2_buf = serialize_pointcloud2(&pointcloud2_data);
                    let pointcloud2_buf_len = pointcloud2_buf.len();
                    let pointcloud2_value = Value::Custom {
                        encoding_descr: String::from("protobuf"),
                        data: ZBuf::from(pointcloud2_buf),
                    };

                    println!(
                        "Mandalay: Putting Pose to {} and Image of {} bytes to {} and PointCloud2 of {} bytes to {}",
                        tagus_resource,
                        image_buf_len,
                        missouri_resource,
                        pointcloud2_buf_len,
                        brazos_resource);
                    workspace
                        .put(&tagus_resource.try_into().unwrap(), pose_value)
                        .await
                        .unwrap();
                    workspace
                        .put(&missouri_resource.try_into().unwrap(), image_value)
                        .await
                        .unwrap();
                    workspace
                        .put(&brazos_resource.try_into().unwrap(), pointcloud2_value)
                        .await
                        .unwrap();
                }
            },
        )
    }
}
