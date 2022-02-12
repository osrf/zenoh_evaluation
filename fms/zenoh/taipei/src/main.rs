use datatypes::*;
use futures::prelude::*;
use futures::select;
use std::env;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap().parse::<i16>().unwrap();

    let port_number = 7519 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let columbia_resource = format!("/{}/columbia", robot_number);
    let mut columbia_subscriber = session.subscribe(&columbia_resource).await.unwrap();

    let colorado_resource = format!("/{}/colorado", robot_number);
    let colorado_expression_id = session.declare_expr(&colorado_resource).await.unwrap();
    session
        .declare_publication(colorado_expression_id)
        .await
        .unwrap();

    let node_name = format!("Taipei_{}", robot_number);
    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = columbia_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!(
                            "{}: Received image of {} bytes from {}, putting it to {}",
                            node_name,
                            image_size,
                            columbia_resource,
                            colorado_resource);
                        let buf = serialize_image(&image);
                        session
                            .put(colorado_expression_id, buf)
                            .await
                            .unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
