use datatypes::*;
use futures::prelude::*;
use futures::select;
use std::convert::TryInto;
use zenoh::*;

#[async_std::main]
async fn main() {
    env_logger::init();

    let zenoh = Zenoh::new(Properties::default().into()).await.unwrap();
    let workspace = zenoh.workspace(None).await.unwrap();

    let mut change_stream = workspace
        .subscribe(&String::from("/status").try_into().unwrap())
        .await
        .unwrap();

    loop {
        select!(
            change = change_stream.next().fuse() => {
                let change = change.unwrap();
                let kind = change.kind;
                match kind {
                    ChangeKind::Put | ChangeKind::Patch => {
                        let buf = match change.value.unwrap() {
                            Value::Custom {encoding_descr: _, data: buf} => Some(buf),
                            _ => None,
                        }.unwrap();
                        let status_size = buf.len();
                        let _status = deserialize_robot_status(buf.contiguous().as_slice()).unwrap();
                        println!("Display: Received status of {} bytes from /status", status_size);
                    },
                    ChangeKind::Delete => {
                        ()
                    },
                }
            }
        )
    }
}
