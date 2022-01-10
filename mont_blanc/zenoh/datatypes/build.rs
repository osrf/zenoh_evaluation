extern crate prost_build;

fn main() {
    prost_build::compile_protos(&["src/data_types.proto"], &["src/"]).unwrap();
}
