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

    let mut config = Properties::default();
    config.insert(String::from("listener"), String::from("tcp/0.0.0.0:7502"));
    let zenoh = Zenoh::new(config.into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    // Inputs
    let mekong_resource_path = format!("/{}/mekong", robot_number);
    let mut mekong_change_stream = workspace
        .subscribe(&mekong_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Outpus
    let lena_resource_path = format!("/{}/lena", robot_number);

    println!("Barcelona: Data generation started");
    let header_data: data_types::Header = random();
    println!("Barcelona: Data generation done");
    println!("Barcelona: Starting loop");
    loop {
        select!(
            change = mekong_change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let twist_with_cov = deserialize_twist_with_covariance_stamped(buf.contiguous().as_slice()).unwrap();
                        let twist = twist_with_cov.twist.unwrap().twist.unwrap();
                        let wrench = data_types::WrenchStamped {
                            header: Some(header_data.clone()),
                            wrench: Some(data_types::Wrench {
                                force: twist.linear,
                                torque: twist.angular,
                            }),
                        };
                        println!("Barcelona: Received TwistWithCovariance from {}, putting WrenchStamped to {}", mekong_resource_path, lena_resource_path);
                        let wrench_buf = serialize_wrench_stamped(&wrench);
                        let wrench_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(wrench_buf),
                        };
                        workspace
                            .put(&lena_resource_path.clone().try_into().unwrap(), wrench_value)
                            .await
                            .unwrap();
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
