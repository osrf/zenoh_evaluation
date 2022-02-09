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
    config.listeners.push("tcp/0.0.0.0:7516".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let tagus_resource = "/tagus";
    let mut tagus_subscriber = session.subscribe(tagus_resource).await.unwrap();
    let danube_resource = "/danube";
    let mut danube_subscriber = session.subscribe(danube_resource).await.unwrap();
    let missouri_resource = "/missouri";
    let mut missouri_subscriber = session.subscribe(missouri_resource).await.unwrap();
    let brazos_resource = "/brazos";
    let mut brazos_subscriber = session.subscribe(brazos_resource).await.unwrap();
    let yamuna_resource = "/yamuna";
    let mut yamuna_subscriber = session.subscribe(yamuna_resource).await.unwrap();
    let godavari_resource = "/godavari";
    let mut godavari_subscriber = session.subscribe(godavari_resource).await.unwrap();
    let loire_resource = "/loire";
    let mut loire_subscriber = session.subscribe(loire_resource).await.unwrap();
    let ohio_resource = "/ohio";
    let mut ohio_subscriber = session.subscribe(ohio_resource).await.unwrap();
    let volga_resource = "/volga";
    let mut volga_subscriber = session.subscribe(volga_resource).await.unwrap();

    // Output resources
    let congo_resource: &str = "/congo";
    let congo_expression_id = session.declare_expr(congo_resource).await.unwrap();
    session
        .declare_publication(congo_expression_id)
        .await
        .unwrap();
    let mekong_resource: &str = "/mekong";
    let mekong_expression_id = session.declare_expr(mekong_resource).await.unwrap();
    session
        .declare_publication(mekong_expression_id)
        .await
        .unwrap();

    println!("Ponce: Data generation started");
    let twist_data: data_types::Twist = random();
    let twist_with_cov_data: data_types::TwistWithCovarianceStamped = random();
    println!("Ponce: Data generation done");

    println!("Ponce: Starting loop");
    loop {
        select!(
            change = tagus_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let _pose = deserialize_pose(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received Pose from {}", tagus_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = danube_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value.payload;
                        println!("Ponce: Received value from {}", danube_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = missouri_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received Image of {} bytes from {}", image_size, missouri_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = brazos_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let pc_size = buf.len();
                        let _pc = deserialize_pointcloud2(buf.contiguous().as_slice()).unwrap();

                        let twist_buf = serialize_twist(&twist_data);

                        let twist_with_cov_buf = serialize_twist_with_covariance_stamped(&twist_with_cov_data);

                        println!("Ponce: Received PointCloud2 of {} bytes from {}, Putting Twist to {} and TwistWithCovariance to {}",
                            pc_size,
                            brazos_resource,
                            congo_resource,
                            mekong_resource);
                        session
                            .put(congo_expression_id, twist_buf)
                            .await
                            .unwrap();
                        session
                            .put(mekong_expression_id, twist_with_cov_buf)
                            .await
                            .unwrap();
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
                        let buf = change.value.payload;
                        let _vec3 = deserialize_vector3(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received Vector3 from {}", yamuna_resource);
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
                        let scan_size = buf.len();
                        let _scan = deserialize_laserscan(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received LaserScan of {} bytes from {}", scan_size, godavari_resource);
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
                        let buf = change.value.payload;
                        let pc_size = buf.len();
                        let _pc = deserialize_pointcloud2(buf.contiguous().as_slice()).unwrap();
                        println!("Ponce: Received PointCloud2 of {} bytes from {}", pc_size, loire_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = ohio_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value.payload;
                        println!("Ponce: Received value from {}", ohio_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = volga_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value.payload;
                        println!("Ponce: Received value from {}", volga_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
