
use std::os::raw::c_int;



extern "C" {
    pub fn allocate_socket() -> c_int;
    pub fn configure_socket(fd: c_int, name: *mut u8) -> c_int;
}