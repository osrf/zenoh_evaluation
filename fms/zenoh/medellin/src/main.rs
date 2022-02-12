use async_std::task;
use rand::random;
use std::env;
use std::time::Duration;
use zenoh::config::Config;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut args_iter = env::args();
    assert_eq!((2, Some(2)), args_iter.size_hint());
    let robot_number = args_iter.nth(1).unwrap().parse::<i16>().unwrap();

    let port_number = 7513 + (robot_number - 1) * 50;
    let listener = format!("tcp/0.0.0.0:{}", port_number);

    let mut config = Config::default();
    config.listeners.push(listener.parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let resource = format!("/{}/nile", robot_number);
    let expression_id = session.declare_expr(&resource).await.unwrap();
    session.declare_publication(expression_id).await.unwrap();

    let node_name = format!("Medellin_{}", robot_number);
    println!("{}: Data generation started", node_name);
    let data = random::<i32>();
    println!("{}: Data generation done", node_name);

    println!("{}: Starting loop", node_name);
    loop {
        println!(
            "{}: Putting generated value to resource {}",
            node_name, resource
        );
        session.put(expression_id, data as i64).await.unwrap();
        task::sleep(Duration::from_millis(10)).await;
    }
}
