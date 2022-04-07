use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7515".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let parana_resource = "/parana";
    let mut parana_subscriber = session.subscribe(parana_resource).await.unwrap();
    let colorado_resource = "/colorado";
    let mut colorado_subscriber = session.subscribe(colorado_resource).await.unwrap();
    let columbia_resource = "/columbia";
    let mut columbia_subscriber = session.subscribe(columbia_resource).await.unwrap();

    // Output resources
    let salween_resource: &str = "/salween";
    let salween_expression_id = session.declare_expr(salween_resource).await.unwrap();
    session
        .declare_publication(salween_expression_id)
        .await
        .unwrap();
    let godavari_resource: &str = "/godavari";
    let godavari_expression_id = session.declare_expr(godavari_resource).await.unwrap();
    session
        .declare_publication(godavari_expression_id)
        .await
        .unwrap();

    println!("Osaka: Data generation started");
    let pointcloud2_data: data_types::PointCloud2 = random();
    let laserscan_data: data_types::LaserScan = random();
    println!("Osaka: Data generation done");

    println!("Osaka: Starting loop");
    loop {
        select!(
            change = parana_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("Osaka: Received value from {}", parana_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = columbia_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Osaka: Received image of {} bytes from {}", image_size, columbia_resource);
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
                            "Osaka: Received image of {} bytes from {}, putting PointCloud2 of {} bytes to {} and LaserScan of {} bytes to {}",
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
