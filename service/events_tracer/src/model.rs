use std::num::ParseFloatError;
mod errors;

pub struct SensorInfo {
    name: String,
    value: f64,
}

impl SensorInfo {
    pub fn as_json_str(&self) -> String {
        String::from("")
    }

    pub fn from_message(topic: &str, payload: &[u8]) -> Result<SensorInfo, ParseMessageError>  {
        let payload_str = String::from_utf8(payload.to_vec());
        let x: f64 = payload_str.as_str().parse::<f64>().; 
        SensorInfo{
            name: String::from(topic), 
            value: x,
        }

    }
}
