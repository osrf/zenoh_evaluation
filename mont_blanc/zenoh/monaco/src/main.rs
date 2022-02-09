use datatypes::*;
use futures::prelude::*;
use futures::select;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7514".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Inputs
    let congo_resource = "/congo";
    let mut congo_subscriber = session.subscribe(congo_resource).await.unwrap();

    // Outpus
    let ohio_resource: &str = "/ohio";
    let ohio_expression_id = session.declare_expr(ohio_resource).await.unwrap();
    session
        .declare_publication(ohio_expression_id)
        .await
        .unwrap();

    println!("Monaco: Starting loop");
    loop {
        select!(
            change = congo_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let buf = change.value;
                        let twist = deserialize_twist(buf.payload.contiguous().as_slice()).unwrap();
                        println!("Monaco: Received Twist from {}, putting value to {}", congo_resource, ohio_resource);
                        session
                            .put(ohio_expression_id, twist.linear.unwrap().x)
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
