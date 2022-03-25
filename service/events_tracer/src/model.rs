use crate::errors;
use std::time::SystemTime;

use errors::EventsTracerError;
use serde::{Serialize, Deserialize};

#[derive(Serialize, Deserialize, Debug)]
pub struct SensorInfo {
    time: SystemTime,
    name: String,
    value: f64,
}

impl SensorInfo {

    pub fn from_message(topic: &str, payload: &[u8]) -> Result<SensorInfo, EventsTracerError>  {
        let payload_str = String::from_utf8(payload.to_vec()).map_err(|_| EventsTracerError::ParseError)?;
        let x: f64 = payload_str.parse::<f64>().map_err(|_| EventsTracerError::ConversionError)?; 
        Ok(SensorInfo{
            time: SystemTime::now(),
            name: String::from(topic), 
            value: x,
        })

    }
}
