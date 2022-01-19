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

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Input resources
    let godavari_resource_path = format!("/{}/godavari", robot_number);
    let mut godavari_change_stream = workspace
        .subscribe(&godavari_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();
    let delhi_resource_path = format!("/{}/delhi", robot_number);
    let mut delhi_change_stream = workspace
        .subscribe(&delhi_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let loire_resource_path = format!("/{}/loire", robot_number);

    println!("Tripoli: Data generation started");
    let pointcloud2_data: data_types::PointCloud2 = random();
    println!("Tripoli: Data generation done");
    println!("Tripoli: Starting loop");
    loop {
        select!(
            change = delhi_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Tripoli: Received image of {} bytes from {}", image_size, delhi_resource_path);
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
                        let laserscan_size = buf.len();
                        let _laserscan = deserialize_laserscan(buf.contiguous().as_slice()).unwrap();

                        let pointcloud2_buf = serialize_pointcloud2(&pointcloud2_data);
                        let pointcloud2_buf_len = pointcloud2_buf.len();
                        let pointcloud2_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(pointcloud2_buf),
                        };

                        println!(
                            "Tripoli: Received laser scan of {} bytes from {}, putting PointCloud2 of {} bytes to {}",
                            laserscan_size,
                            godavari_resource_path,
                            pointcloud2_buf_len,
                            loire_resource_path);
                        workspace
                            .put(&loire_resource_path.clone().try_into().unwrap(), pointcloud2_value)
                            .await
                            .unwrap();
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
