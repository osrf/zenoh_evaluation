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

    let mut subscriber = session.subscribe("/arkansas").await.unwrap();

    println!("Arequipa: Starting loop");
    loop {
        select!(
            change = subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("Arequipa: Received value from /arkansas");
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
