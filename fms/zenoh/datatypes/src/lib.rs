use prost::Message;
use rand::distributions::{Alphanumeric, Distribution, Standard};
use rand::Rng;
use std::io::Cursor;
use std::time::{SystemTime, UNIX_EPOCH};

pub mod data_types {
    include!(concat!(env!("OUT_DIR"), "/datatypes.data_types.rs"));
}

pub fn random_string(length: usize) -> String {
    rand::thread_rng()
        .sample_iter(Alphanumeric)
        .take(length)
        .map(char::from)
        .collect()
}

pub fn random_bytes(length: usize) -> Vec<u8> {
    (0..length).map(|_| rand::random::<u8>()).collect()
}

pub fn random_floats(length: usize) -> Vec<f32> {
    (0..length).map(|_| rand::random::<f32>()).collect()
}

pub fn random_doubles(length: usize) -> Vec<f64> {
    (0..length).map(|_| rand::random::<f64>()).collect()
}

impl Distribution<data_types::Header> for Standard {
    fn sample<R: Rng + ?Sized>(&self, _rng: &mut R) -> data_types::Header {
        let now = SystemTime::now();
        let now_as_duration = now
            .duration_since(UNIX_EPOCH)
            .expect("System time went backwards");
        data_types::Header {
            sec: now_as_duration.as_secs() as i32,
            nanosec: now_as_duration.subsec_nanos(),
            frame_id: random_string(16),
        }
    }
}

impl Distribution<data_types::Point> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Point {
        data_types::Point {
            x: rng.gen(),
            y: rng.gen(),
            z: rng.gen(),
        }
    }
}

pub fn serialize_point(point: &data_types::Point) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(point.encoded_len());
    point.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_point(buf: &[u8]) -> Result<data_types::Point, prost::DecodeError> {
    data_types::Point::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::Quaternion> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Quaternion {
        data_types::Quaternion {
            x: rng.gen(),
            y: rng.gen(),
            z: rng.gen(),
            w: rng.gen(),
        }
    }
}

pub fn serialize_quaternion(quat: &data_types::Quaternion) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(quat.encoded_len());
    quat.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_quaternion(buf: &[u8]) -> Result<data_types::Quaternion, prost::DecodeError> {
    data_types::Quaternion::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::Vector3> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Vector3 {
        data_types::Vector3 {
            x: rng.gen(),
            y: rng.gen(),
            z: rng.gen(),
        }
    }
}

pub fn serialize_vector3(vec3: &data_types::Vector3) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(vec3.encoded_len());
    vec3.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_vector3(buf: &[u8]) -> Result<data_types::Vector3, prost::DecodeError> {
    data_types::Vector3::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::Vector3Stamped> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Vector3Stamped {
        data_types::Vector3Stamped {
            header: rng.gen(),
            vector: rng.gen(),
        }
    }
}

pub fn serialize_vector3_stamped(vec3s: &data_types::Vector3Stamped) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(vec3s.encoded_len());
    vec3s.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_vector3_stamped(
    buf: &[u8],
) -> Result<data_types::Vector3Stamped, prost::DecodeError> {
    data_types::Vector3Stamped::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::Pose> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Pose {
        data_types::Pose {
            position: rng.gen(),
            orientation: rng.gen(),
        }
    }
}

pub fn serialize_pose(pose: &data_types::Pose) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(pose.encoded_len());
    pose.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_pose(buf: &[u8]) -> Result<data_types::Pose, prost::DecodeError> {
    data_types::Pose::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::PoseWithCovariance> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::PoseWithCovariance {
        data_types::PoseWithCovariance {
            pose: rng.gen(),
            covariance: random_doubles(64),
        }
    }
}

pub fn serialize_pose_with_covariance(pose_with_cov: &data_types::PoseWithCovariance) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(pose_with_cov.encoded_len());
    pose_with_cov.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_pose_with_covariance(
    buf: &[u8],
) -> Result<data_types::PoseWithCovariance, prost::DecodeError> {
    data_types::PoseWithCovariance::decode(&mut Cursor::new(buf))
}

pub fn random_poses(length: usize) -> Vec<data_types::PoseWithCovariance> {
    (0..length)
        .map(|_| rand::random::<data_types::PoseWithCovariance>())
        .collect()
}

impl Distribution<data_types::Twist> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Twist {
        data_types::Twist {
            linear: rng.gen(),
            angular: rng.gen(),
        }
    }
}

pub fn serialize_twist(twist: &data_types::Twist) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(twist.encoded_len());
    twist.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_twist(buf: &[u8]) -> Result<data_types::Twist, prost::DecodeError> {
    data_types::Twist::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::TwistWithCovariance> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::TwistWithCovariance {
        data_types::TwistWithCovariance {
            twist: rng.gen(),
            covariance: random_doubles(36),
        }
    }
}

pub fn serialize_twist_with_covariance(
    twist_with_cov: &data_types::TwistWithCovariance,
) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(twist_with_cov.encoded_len());
    twist_with_cov.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_twist_with_covariance(
    buf: &[u8],
) -> Result<data_types::TwistWithCovariance, prost::DecodeError> {
    data_types::TwistWithCovariance::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::TwistWithCovarianceStamped> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::TwistWithCovarianceStamped {
        data_types::TwistWithCovarianceStamped {
            header: rng.gen(),
            twist: rng.gen(),
        }
    }
}

pub fn serialize_twist_with_covariance_stamped(
    twist_with_cov: &data_types::TwistWithCovarianceStamped,
) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(twist_with_cov.encoded_len());
    twist_with_cov.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_twist_with_covariance_stamped(
    buf: &[u8],
) -> Result<data_types::TwistWithCovarianceStamped, prost::DecodeError> {
    data_types::TwistWithCovarianceStamped::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::Wrench> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::Wrench {
        data_types::Wrench {
            force: rng.gen(),
            torque: rng.gen(),
        }
    }
}

pub fn serialize_wrench(wrench: &data_types::Wrench) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(wrench.encoded_len());
    wrench.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_wrench(buf: &[u8]) -> Result<data_types::Wrench, prost::DecodeError> {
    data_types::Wrench::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::WrenchStamped> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::WrenchStamped {
        data_types::WrenchStamped {
            header: rng.gen(),
            wrench: rng.gen(),
        }
    }
}

pub fn serialize_wrench_stamped(wrench_stamped: &data_types::WrenchStamped) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(wrench_stamped.encoded_len());
    wrench_stamped.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_wrench_stamped(
    buf: &[u8],
) -> Result<data_types::WrenchStamped, prost::DecodeError> {
    data_types::WrenchStamped::decode(&mut Cursor::new(buf))
}

impl Distribution<data_types::RobotStatus> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> data_types::RobotStatus {
        data_types::RobotStatus {
            header: rng.gen(),
            name: random_string(16),
            model: random_string(16),
            task_id: random_string(8),
            mode: rng.gen(),
            battery: rng.gen(),
            current_pose: rng.gen(),
            path: random_poses(32),
        }
    }
}

pub fn serialize_robot_status(status: &data_types::RobotStatus) -> Vec<u8> {
    let mut buf = Vec::new();
    buf.reserve(status.encoded_len());
    status.encode(&mut buf).unwrap();
    buf
}

pub fn deserialize_robot_status(buf: &[u8]) -> Result<data_types::RobotStatus, prost::DecodeError> {
    data_types::RobotStatus::decode(&mut Cursor::new(buf))
}
