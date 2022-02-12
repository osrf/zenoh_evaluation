use datatypes::*;
use futures::prelude::*;
use futures::select;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7501".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource = "/processed";
    let mut subscriber = session.subscribe(resource).await.unwrap();

    println!("Display: Starting loop");
    loop {
        select!(
            change = subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value.payload;
                        let image_size = buf.len();
                        let _image = deserialize_image(buf.contiguous().as_slice()).unwrap();
                        println!("Display: Received image of {} bytes from /processed", image_size);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
