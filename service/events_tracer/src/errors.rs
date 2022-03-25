use std::fmt;

#[derive(Debug, Clone)]
pub enum EventsTracerError {
    ParseError,
    DeserializationError,
    ConversionError,
    IOError,
  }

impl std::error::Error for EventsTracerError {}


impl fmt::Display for EventsTracerError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            EventsTracerError::ParseError => write! (f, "Parse Error"),
            EventsTracerError::DeserializationError => write! (f, "Deserialization Error"),
            EventsTracerError::ConversionError => write!(f, "Conversion Error"),
            EventsTracerError::IOError => write!(f, "IO Error"),
        }
    }
}