use datatypes::*;
use futures::prelude::*;
use futures::select;
use rand::random;
use std::time::Instant;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7512".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let danube_resource = "/danube";
    let mut danube_subscriber = session.subscribe(danube_resource).await.unwrap();
    let chenab_resource = "/chenab";
    let mut chenab_subscriber = session.subscribe(chenab_resource).await.unwrap();
    let salween_resource = "/salween";
    let mut salween_subscriber = session.subscribe(salween_resource).await.unwrap();
    let godavari_resource = "/godavari";
    let mut godavari_subscriber = session.subscribe(godavari_resource).await.unwrap();
    let yamuna_resource = "/yamuna";
    let mut yamuna_subscriber = session.subscribe(yamuna_resource).await.unwrap();
    let loire_resource = "/loire";
    let mut loire_subscriber = session.subscribe(loire_resource).await.unwrap();

    // Output resources
    let tagus_resource: &str = "/tagus";
    let tagus_expression_id = session.declare_expr(tagus_resource).await.unwrap();
    session
        .declare_publication(tagus_expression_id)
        .await
        .unwrap();
    let missouri_resource: &str = "/missouri";
    let missouri_expression_id = session.declare_expr(missouri_resource).await.unwrap();
    session
        .declare_publication(missouri_expression_id)
        .await
        .unwrap();
    let brazos_resource: &str = "/brazos";
    let brazos_expression_id = session.declare_expr(brazos_resource).await.unwrap();
    session
        .declare_publication(brazos_expression_id)
        .await
        .unwrap();

    println!("Mandalay: Data generation started");
    let pose_data: data_types::Pose = random();
    let image_data: data_types::Image = random();
    let pointcloud2_data: data_types::PointCloud2 = random();
    println!("Mandalay: Data generation done");

    println!("Mandalay: Starting loop");
    let mut start_time = Instant::now();
    loop {
        select!(
            change = danube_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("Mandalay: Received value from {}", danube_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = chenab_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _quat = deserialize_quaternion(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received value from {}", chenab_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = salween_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let pc_size = buf.payload.len();
                        let _pc = deserialize_pointcloud2(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received PointCloud2 of {} bytes from {}", pc_size, salween_resource);
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
                        let buf = change.value;
                        let scan_size = buf.payload.len();
                        let _scan = deserialize_laserscan(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received image of {} bytes from {}", scan_size, godavari_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = yamuna_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let _vec3 = deserialize_vector3(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received Vector3 from {}", yamuna_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = loire_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let pc_size = buf.payload.len();
                        let _pc = deserialize_pointcloud2(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Mandalay: Received PointCloud2 of {} bytes from {}", pc_size, loire_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            default => {
                if start_time.elapsed().as_millis() > 100 {
                    start_time = Instant::now();

                    let pose_buf = serialize_pose(&pose_data);

                    let image_buf = serialize_image(&image_data);
                    let image_buf_len = image_buf.len();

                    let pointcloud2_buf = serialize_pointcloud2(&pointcloud2_data);
                    let pointcloud2_buf_len = pointcloud2_buf.len();

                    println!(
                        "Mandalay: Putting Pose to {} and Image of {} bytes to {} and PointCloud2 of {} bytes to {}",
                        tagus_resource,
                        image_buf_len,
                        missouri_resource,
                        pointcloud2_buf_len,
                        brazos_resource);
                    session
                        .put(tagus_expression_id, pose_buf)
                        .await
                        .unwrap();
                    session
                        .put(missouri_expression_id, image_buf)
                        .await
                        .unwrap();
                    session
                        .put(brazos_expression_id, pointcloud2_buf)
                        .await
                        .unwrap();
                }
            },
        )
    }
}
