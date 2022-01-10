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
    let mut parana_change_stream = workspace
        .subscribe(&String::from("/parana").try_into().unwrap())
        .await
        .unwrap();
    let mut colorado_change_stream = workspace
        .subscribe(&String::from("/colorado").try_into().unwrap())
        .await
        .unwrap();
    let mut delhi_change_stream = workspace
        .subscribe(&String::from("/delhi").try_into().unwrap())
        .await
        .unwrap();

    // Output resources
    let salween_resource: &str = "/salween";
    let godavari_resource: &str = "/godavari";

    println!("Osaka: Data generation started");
    let pointcloud2_data: data_types::PointCloud2 = random();
    let laserscan_data: data_types::LaserScan = random();
    println!("Osaka: Data generation done");
    println!("Osaka: Starting loop");
    loop {
        select!(
            change = parana_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let _data = change.value.unwrap();
                        println!("Osaka: Received value from /parana");
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
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
                        println!("Osaka: Received image of {} bytes from /delhi", image_size);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            },
            change = colorado_change_stream.next().fuse() => {
                let change = change.unwrap();
                match change.kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();

                        let pointcloud2_buf = serialize_pointcloud2(&pointcloud2_data);
                        let pointcloud2_buf_len = pointcloud2_buf.len();
                        let pointcloud2_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(pointcloud2_buf),
                        };

                        let laserscan_buf = serialize_laserscan(&laserscan_data);
                        let laserscan_buf_len = laserscan_buf.len();
                        let laserscan_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(laserscan_buf),
                        };
                        println!(
                            "Osaka: Received image of {} bytes from /columbia, putting PointCloud2 of {} bytes to {} and LaserScan of {} bytes to {}",
                            image_size,
                            pointcloud2_buf_len,
                            salween_resource,
                            laserscan_buf_len,
                            godavari_resource);
                        workspace
                            .put(&salween_resource.try_into().unwrap(), pointcloud2_value)
                            .await
                            .unwrap();
                        workspace
                            .put(&godavari_resource.try_into().unwrap(), laserscan_value)
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
