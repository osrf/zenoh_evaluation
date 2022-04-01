use bigdata::*;
use futures::prelude::*;
use futures::select;
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};
use zenoh::config::Config;
use zenoh::prelude::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let mut config = Config::default();
    config.listen.endpoints.push("tcp/0.0.0.0:7502".parse().unwrap());
    let session = zenoh::open(config).await.unwrap();

    let mut subscriber = session.subscribe("/amazon").await.unwrap();

    println!("Received at,Transmission time (s),Transmitted (Bytes),Rate (B/s),Bandwidth (bps),Bandwidth (Mbps)");
    let mut start_time = Instant::now();
    loop {
        select!(
            sample = subscriber.next() => {
                let sample = sample .unwrap();
                let kind = sample.kind;
                match kind {
                    SampleKind::Put | SampleKind::Patch => {
                        let mut start_instant = Instant::now();
                        let buf = sample.value.payload.contiguous();
                        println!(
                            "Buffer retrieval took {}",
                            start_instant.elapsed().as_secs_f64()
                        );
                        let transmission_size = buf.len();
                        start_instant = Instant::now();
                        let big_d = deserialize_big_data(&buf).unwrap();
                        println!(
                            "Deserialisation took {}",
                            start_instant.elapsed().as_secs_f64()
                        );
                        let transmission_finish_st = SystemTime::now();
                        let big_data::Timestamp { sec: data_ts_sec, nanosec: data_ts_nanosec } = big_d.timestamp.unwrap();
                        let transmission_start = Duration::new(data_ts_sec, data_ts_nanosec);
                        let transmission_finish = transmission_finish_st
                            .duration_since(UNIX_EPOCH)
                            .expect("System time went backwards");
                        let transmission_time = transmission_finish - transmission_start;
                        let transmission_rate: f64 =
                            transmission_size as f64 / transmission_time.as_secs_f64();
                        let transmission_bandwidth: f64 = transmission_rate * 8.0;
                        println!(
                            "{}.{:09},{:?},{},{},{},{}",
                            transmission_finish.as_secs(),
                            transmission_finish.subsec_nanos(),
                            transmission_time,
                            transmission_size,
                            transmission_rate,
                            transmission_bandwidth,
                            transmission_bandwidth / 1024.0 / 1024.0);
                     },
                    SampleKind::Delete => {
                        println!("Received {:?} for {} with timestamp {:?}",
                            kind, sample.key_expr, sample.timestamp);
                    },
                };
            },
            default => {
                if start_time.elapsed().as_millis() > 120000 {
                    start_time = Instant::now();
                    let timeout_st = SystemTime::now();
                    let timeout = timeout_st
                        .duration_since(UNIX_EPOCH)
                        .expect("System time went backwards");
                    println!(
                        "{}.{:09},{:?},{},{},{},{}",
                        timeout.as_secs(),
                        timeout.subsec_nanos(),
                        "60s",
                        0,
                        0.0,
                        0.0,
                        0.0);
                }
            }
        )
    }
}
