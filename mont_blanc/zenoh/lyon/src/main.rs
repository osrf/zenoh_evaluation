use futures::prelude::*;
use futures::select;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7511".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let amazon_resource = "/amazon";
    let mut amazon_subscriber = session.subscribe(amazon_resource).await.unwrap();

    let tigris_resource: &str = "/tigris";
    let tigris_expression_id = session.declare_expr(tigris_resource).await.unwrap();
    session
        .declare_publication(tigris_expression_id)
        .await
        .unwrap();

    println!("Lyon: Starting loop");
    loop {
        select!(
            change = amazon_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let data = change.value;
                        println!("Lyon: Received value, putting it to resource {}", tigris_resource);
                        session.put(tigris_expression_id, data).await.unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
