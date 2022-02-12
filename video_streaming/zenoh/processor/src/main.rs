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
    config.listeners.push("tcp/0.0.0.0:7502".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource = "/camera";
    let mut subscriber = session.subscribe(resource).await.unwrap();

    let output_resource = "/processed";
    let expression_id = session.declare_expr(output_resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    println!("Processor: Starting loop");
    loop {
        select!(
            change = subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let mut image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Processor: Received image of {} bytes from /camera, putting it to {}", image_size, output_resource);
                        image.header = random();
                        let buf = serialize_image(&image);
                        session.put(expression_id, buf).await.unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
