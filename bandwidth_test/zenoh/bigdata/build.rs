extern crate prost_build;

fn main() {
    prost_build::compile_protos(&["src/big_data.proto"], &["src/"]).unwrap();
}
