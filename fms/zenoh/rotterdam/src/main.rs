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

    // Inputs
    let mekong_resource_path = format!("/{}/mekong", robot_number);
    let mut mekong_change_stream = workspace
        .subscribe(&mekong_resource_path.clone().try_into().unwrap())
        .await
        .unwrap();

    // Outpus
    let murray_resource_path = format!("/{}/murray", robot_number);

    println!("Rotterdam: Data generation started");
    let header_data: data_types::Header = random();
    println!("Rotterdam: Data generation done");
    println!("Rotterdam: Starting loop");
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
                        let vec3s = data_types::Vector3Stamped {
                            header: Some(header_data.clone()),
                            vector: twist_with_cov.twist.unwrap().twist.unwrap().linear,
                        };
                        println!("Rotterdam: Received TwistWithCovariance from {}, putting Vector3Stamped to {}", mekong_resource_path, murray_resource_path);
                        let vec3s_buf = serialize_vector3_stamped(&vec3s);
                        let vec3s_value = Value::Custom {
                            encoding_descr: String::from("protobuf"),
                            data: ZBuf::from(vec3s_buf),
                        };
                        workspace
                            .put(&murray_resource_path.clone().try_into().unwrap(), vec3s_value)
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
