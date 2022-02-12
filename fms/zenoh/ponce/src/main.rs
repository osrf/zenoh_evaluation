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

    let port_number = 7516 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let tagus_resource = format!("/{}/tagus", robot_number);
    let mut tagus_subscriber = session.subscribe(&tagus_resource).await.unwrap();
    let danube_resource = format!("/{}/danube", robot_number);
    let mut danube_subscriber = session.subscribe(&danube_resource).await.unwrap();
    let missouri_resource = format!("/{}/missouri", robot_number);
    let mut missouri_subscriber = session.subscribe(&missouri_resource).await.unwrap();
    let brazos_resource = format!("/{}/brazos", robot_number);
    let mut brazos_subscriber = session.subscribe(&brazos_resource).await.unwrap();
    let yamuna_resource = format!("/{}/yamuna", robot_number);
    let mut yamuna_subscriber = session.subscribe(&yamuna_resource).await.unwrap();
    let godavari_resource = format!("/{}/godavari", robot_number);
    let mut godavari_subscriber = session.subscribe(&godavari_resource).await.unwrap();
    let loire_resource = format!("/{}/loire", robot_number);
    let mut loire_subscriber = session.subscribe(&loire_resource).await.unwrap();
    let ohio_resource = format!("/{}/ohio", robot_number);
    let mut ohio_subscriber = session.subscribe(&ohio_resource).await.unwrap();
    let volga_resource = format!("/{}/volga", robot_number);
    let mut volga_subscriber = session.subscribe(&volga_resource).await.unwrap();

    // Output resources
    let congo_resource = format!("/{}/congo", robot_number);
    let congo_expression_id = session.declare_expr(&congo_resource).await.unwrap();
    session
        .declare_publication(congo_expression_id)
        .await
        .unwrap();
    let mekong_resource = format!("/{}/mekong", robot_number);
    let mekong_expression_id = session.declare_expr(&mekong_resource).await.unwrap();
    session
        .declare_publication(mekong_expression_id)
        .await
        .unwrap();

    let node_name = format!("Ponce_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let twist_data: data_types::Twist = random();
    let twist_with_cov_data: data_types::TwistWithCovarianceStamped = random();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = tagus_subscriber.next() => {
                let change = change.unwrap();
                match change.kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let _pose = deserialize_pose(buf.contiguous().as_slice()).unwrap();
                        println!("{}: Received Pose from {}", node_name, tagus_resource);
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
                        println!("{}: Received value from {}", node_name, danube_resource);
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
                        println!("{}: Received Image of {} bytes from {}", node_name, image_size, missouri_resource);
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

                        println!("{}: Received PointCloud2 of {} bytes from {}, Putting Twist to {} and TwistWithCovariance to {}", node_name,
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
                        println!("{}: Received Vector3 from {}", node_name, yamuna_resource);
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
                        println!("{}: Received LaserScan of {} bytes from {}", node_name, scan_size, godavari_resource);
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
                        println!("{}: Received PointCloud2 of {} bytes from {}", node_name, pc_size, loire_resource);
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
                        println!("{}: Received value from {}", node_name, ohio_resource);
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
                        println!("{}: Received value from {}", node_name, volga_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
