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
    config.listeners.push("tcp/0.0.0.0:7520".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let godavari_resource = "/godavari";
    let mut godavari_subscriber = session.subscribe(godavari_resource).await.unwrap();
    let delhi_resource = "/delhi";
    let mut delhi_subscriber = session.subscribe(delhi_resource).await.unwrap();

    // Output resources
    let loire_resource: &str = "/loire";
    let loire_expression_id = session.declare_expr(loire_resource).await.unwrap();
    session
        .declare_publication(loire_expression_id)
        .await
        .unwrap();

    println!("Tripoli: Data generation started");
    let pointcloud2_data: data_types::PointCloud2 = random();
    println!("Tripoli: Data generation done");

    println!("Tripoli: Starting loop");
    loop {
        select!(
            change = delhi_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Tripoli: Received image of {} bytes from {}", image_size, delhi_resource);
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
                            "Tripoli: Received laser scan of {} bytes from {}, putting PointCloud2 of {} bytes to {}",
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
