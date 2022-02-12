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

    let port_number = 7515 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let parana_resource = format!("/{}/parana", robot_number);
    let mut parana_subscriber = session.subscribe(&parana_resource).await.unwrap();
    let colorado_resource = format!("/{}/colorado", robot_number);
    let mut colorado_subscriber = session.subscribe(&colorado_resource).await.unwrap();
    let delhi_resource = format!("/{}/delhi", robot_number);
    let mut delhi_subscriber = session.subscribe(&delhi_resource).await.unwrap();

    // Output resources
    let salween_resource = format!("/{}/salween", robot_number);
    let salween_expression_id = session.declare_expr(&salween_resource).await.unwrap();
    session
        .declare_publication(salween_expression_id)
        .await
        .unwrap();
    let godavari_resource = format!("/{}/godavari", robot_number);
    let godavari_expression_id = session.declare_expr(&godavari_resource).await.unwrap();
    session
        .declare_publication(godavari_expression_id)
        .await
        .unwrap();

    let node_name = format!("Osaka_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let pointcloud2_data: data_types::PointCloud2 = random();
    let laserscan_data: data_types::LaserScan = random();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = parana_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("{}: Received value from {}", node_name, parana_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
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
            change = colorado_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();

                        let pointcloud2_buf = serialize_pointcloud2(&pointcloud2_data);
                        let pointcloud2_buf_len = pointcloud2_buf.len();

                        let laserscan_buf = serialize_laserscan(&laserscan_data);
                        let laserscan_buf_len = laserscan_buf.len();

                        println!(
                            "{}: Received image of {} bytes from {}, putting PointCloud2 of {} bytes to {} and LaserScan of {} bytes to {}",
                            node_name,
                            image_size,
                            colorado_resource,
                            pointcloud2_buf_len,
                            salween_resource,
                            laserscan_buf_len,
                            godavari_resource);
                        session
                            .put(salween_expression_id, pointcloud2_buf)
                            .await
                            .unwrap();
                        session
                            .put(godavari_expression_id, laserscan_buf)
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
