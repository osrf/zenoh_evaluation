use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::env;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap().parse::<i16>().unwrap();

    let port_number = 7520 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let godavari_resource = format!("/{}/godavari", robot_number);
    let mut godavari_subscriber = session.subscribe(&godavari_resource).await.unwrap();
    let delhi_resource = format!("/{}/delhi", robot_number);
    let mut delhi_subscriber = session.subscribe(&delhi_resource).await.unwrap();

    // Output resources
    let loire_resource = format!("/{}/loire", robot_number);
    let loire_expression_id = session.declare_expr(&loire_resource).await.unwrap();
    session
        .declare_publication(loire_expression_id)
        .await
        .unwrap();

    let node_name = format!("Tripoli_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let pointcloud2_data: data_types::PointCloud2 = random();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = delhi_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("{}: Received image of {} bytes from {}", node_name, image_size, delhi_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = godavari_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let laserscan_size = buf.len();
                        let _laserscan = deserialize_laserscan(buf.contiguous().as_slice()).unwrap();

                        let pointcloud2_buf = serialize_pointcloud2(&pointcloud2_data);
                        let pointcloud2_buf_len = pointcloud2_buf.len();

                        println!(
                            "{}: Received laser scan of {} bytes from {}, putting PointCloud2 of {} bytes to {}",
                            node_name,
                            laserscan_size,
                            godavari_resource,
                            pointcloud2_buf_len,
                            loire_resource);
                        session
                            .put(loire_expression_id, pointcloud2_buf)
                            .await
                            .unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
