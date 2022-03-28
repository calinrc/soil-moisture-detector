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



use clap::Parser;

/// Simple program to greet a person
#[derive(Parser, Debug)]
#[clap(author, version, about, long_about = None)]
pub struct Args {
    /// Mqtt broker host
    #[clap(short, long)]
    mqtt_broker: String,

    /// Output json line file path
    #[clap(short, long)]
    output_json_path: String,
    
    // /// Number of times to greet
    // #[clap(short, long, default_value_t = 1)]
    // count: u8,
}
