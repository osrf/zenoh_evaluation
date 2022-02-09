use futures::prelude::*;
use futures::select;
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listeners.push("tcp/0.0.0.0:7508".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    // Input resources
    let tigris_resource = "/tigris";
    let mut tigris_subscriber = session.subscribe(tigris_resource).await.unwrap();
    let ganges_resource = "/ganges";
    let mut ganges_subscriber = session.subscribe(ganges_resource).await.unwrap();
    let nile_resource = "/nile";
    let mut nile_subscriber = session.subscribe(nile_resource).await.unwrap();
    let danube_resource = "/danube";
    let mut danube_subscriber = session.subscribe(danube_resource).await.unwrap();

    // Output resource
    let parana_resource: &str = "/parana";
    let parana_expression_id = session.declare_expr(parana_resource).await.unwrap();
    session
        .declare_publication(parana_expression_id)
        .await
        .unwrap();

    println!("Hamburg: Starting loop");
    loop {
        select!(
            change = tigris_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        println!("Hamburg: Received value from {}", tigris_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = ganges_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        println!("Hamburg: Received value from {}", ganges_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = nile_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        println!("Hamburg: Received value from {}", nile_resource);
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
            change = danube_subscriber.next() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let data = change.value;
                        println!("Hamburg: Received value from {}; putting it to {}", nile_resource, parana_resource);
                        session.put(parana_expression_id, data).await.unwrap();
                    },
                    SampleKind::Delete => {
                        ()
                    },
                }
            },
        )
    }
}
