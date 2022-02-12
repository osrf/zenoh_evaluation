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

    let port_number = 7501 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let mut subscriber = session
        .subscribe(format!("/{}/arkansas", robot_number))
        .await
        .unwrap();

    let node_name = format!("Arequipa_{}", robot_number);
    println!("{}: Starting loop", node_name);
    loop {
        select!(
            change = subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let _data = change.value;
                        println!("{}: Received value from /arkansas", node_name);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
