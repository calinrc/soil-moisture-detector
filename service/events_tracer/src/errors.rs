
#[derive(Debug, Clone)]
struct ParseMessageError;

impl fmt::Display for ParseMessageError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Invalid Message information")
    }
}