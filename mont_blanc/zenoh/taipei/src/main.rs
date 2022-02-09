use datatypes::*;
use futures::prelude::*;
use futures::select;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7519".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let columbia_resource = "/columbia";
    let mut columbia_subscriber = session.subscribe(columbia_resource).await.unwrap();

    let colorado_resource: &str = "/colorado";
    let colorado_expression_id = session.declare_expr(colorado_resource).await.unwrap();
    session
        .declare_publication(colorado_expression_id)
        .await
        .unwrap();

    println!("Taipei: Starting loop");
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
                            "Taipei: Received image of {} bytes from {}, putting it to {}",
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
