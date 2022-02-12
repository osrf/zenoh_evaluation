use prost::Message;
use rand::distributions::{Alphanumeric, Distribution, Standard};
use rand::Rng;
use std::io::Cursor;
use std::time::{SystemTime, UNIX_EPOCH};

pub mod big_data {
    include!(concat!(env!("OUT_DIR"), "/bigdata.big_data.rs"));
}

fn random_string(length: usize) -> String {
    rand::thread_rng()
        .sample_iter(Alphanumeric)
        .take(length)
        .map(char::from)
        .collect()
}

fn random_bytes(length: usize) -> Vec<u8> {
    (0..length).map(|_| rand::random::<u8>()).collect()
}

fn empty() -> Vec<u8> {
    Vec::new()
}

impl Distribution<big_data::Header> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> big_data::Header {
        big_data::Header {
            sec: rng.gen(),
            nanosec: rng.gen(),
            frame_id: random_string(16),
        }
    }
}

impl Distribution<big_data::BasicTypes> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> big_data::BasicTypes {
        big_data::BasicTypes {
            bool_value: rng.gen(),
            int32_value: rng.gen(),
            uint32_value: rng.gen(),
            int64_value: rng.gen(),
            uint64_value: rng.gen(),
            float_value: rng.gen(),
            double_value: rng.gen(),
            string_value: random_string(128),
        }
    }
}

fn random_basic_types(length: usize) -> Vec<big_data::BasicTypes> {
    (0..length)
        .map(|_| rand::random::<big_data::BasicTypes>())
        .collect()
}

impl Distribution<big_data::Image> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> big_data::Image {
        big_data::Image {
            header: rng.gen(),
            height: rng.gen(),
            width: rng.gen(),
            encoding: random_string(32),
            is_bigendian: rng.gen(),
            step: rng.gen(),
            data: random_bytes(1920 * 1080 * 3),
            //data: empty(),
        }
    }
}

impl Distribution<big_data::point_cloud2::point_field::DataType> for Standard {
    fn sample<R: Rng + ?Sized>(
        &self,
        rng: &mut R,
    ) -> big_data::point_cloud2::point_field::DataType {
        match rng.gen_range(0..=7) {
            0 => big_data::point_cloud2::point_field::DataType::Int8,
            1 => big_data::point_cloud2::point_field::DataType::Uint8,
            2 => big_data::point_cloud2::point_field::DataType::Int16,
            3 => big_data::point_cloud2::point_field::DataType::Uint16,
            4 => big_data::point_cloud2::point_field::DataType::Int32,
            5 => big_data::point_cloud2::point_field::DataType::Uint32,
            6 => big_data::point_cloud2::point_field::DataType::Float32,
            7 => big_data::point_cloud2::point_field::DataType::Float64,
            _ => big_data::point_cloud2::point_field::DataType::Int8,
        }
    }
}

impl Distribution<big_data::point_cloud2::PointField> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> big_data::point_cloud2::PointField {
        big_data::point_cloud2::PointField {
            name: random_string(32),
            offset: rng.gen(),
            datatype: rng.gen(),
            count: rng.gen(),
        }
    }
}

fn random_point_fields(length: usize) -> Vec<big_data::point_cloud2::PointField> {
    (0..length)
        .map(|_| rand::random::<big_data::point_cloud2::PointField>())
        .collect()
}

impl Distribution<big_data::PointCloud2> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> big_data::PointCloud2 {
        big_data::PointCloud2 {
            header: rng.gen(),
            height: rng.gen(),
            width: rng.gen(),
            fields: random_point_fields(3),
            is_bigendian: rng.gen(),
            point_step: rng.gen(),
            row_step: rng.gen(),
            data: random_bytes(4 * 4 * 4 * 1280 * 960),
            //data: empty(),
            is_dense: rng.gen(),
        }
    }
}

impl Distribution<big_data::BigData> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> big_data::BigData {
        big_data::BigData {
            timestamp: Some(big_data::Timestamp::default()),
            bool_value: rng.gen(),
            int32_value: rng.gen(),
            uint32_value: rng.gen(),
            int64_value: rng.gen(),
            uint64_value: rng.gen(),
            float_value: rng.gen(),
            double_value: rng.gen(),
            string_value: random_string(256),
            basic_types_values: random_basic_types(3),
            image_value: rng.gen(),
            point_cloud_value: rng.gen(),
        }
    }
}

pub fn create_big_data() -> big_data::BigData {
    let mut rng = rand::thread_rng();
    rng.gen()
}

pub fn set_big_data_timestamp_to_now(big_d: &mut big_data::BigData) {
    let now = SystemTime::now();
    let duration = now
        .duration_since(UNIX_EPOCH)
        .expect("System time went backwards");
    big_d.timestamp = Some(big_data::Timestamp {
        sec: duration.as_secs(),
        nanosec: duration.subsec_nanos(),
    });
}

pub fn serialize_big_data(big_d: &big_data::BigData) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(big_d.encoded_len());
    big_d.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_big_data(buf: &[u8]) -> Result<big_data::BigData, prost::DecodeError> {
    big_data::BigData::decode(&mut Cursor::new(buf))
}
