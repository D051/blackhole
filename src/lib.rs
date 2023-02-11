use std::fs::File;

use std::io::Read;
use std::io::Write;
// use std::ffi::CStr;
use std::os::fd::AsRawFd;
use std::os::fd::FromRawFd;
// use std::os::raw::c_char;
// use std::os::raw::c_int;

mod plattform;

// Adapter struct
pub struct Adapter {
    file: File,
    display_name: String,
    adapter_name: String,
}

// debug implementation
impl Adapter {
    // Get the display name
    pub fn print(&self) {
        println!("--------------------------------");
        println!("<> File: {}", self.file.as_raw_fd());
        println!("<> Display Name: {}", self.display_name);
        println!("<> Adapter Name: {}", self.adapter_name);
        println!("--------------------------------");
    }
}

// constructor implementation
impl Adapter {
    // Create a new adapter
    pub fn new(name: &str) -> Adapter {
        // -----------------------------
        // -- ALLOCATE THE TUN SOCKET --
        // -----------------------------
        // allocate the socket
        let fd: i32 = unsafe { plattform::allocate_socket() };
        // check if the socket allocation was successful
        if fd < 0 {
            panic!("Socket allocation failed");
        }
        // ------------------------------
        // -- CONFIGURE THE TUN SOCKET --
        // ------------------------------
        // allocate name buffer
        let mut name_buf: [u8; 32] = [0; 32];
        // copy the name to the buffer
        name_buf[0..name.len()].clone_from_slice(name.as_bytes());
        // get a pointer to the name buffer
        let name_ptr: *mut u8 = name_buf.as_mut_ptr();
        // configure the socket
        let result: i32 = unsafe { plattform::configure_socket(fd, name_ptr) };
        // check if the socket configuration was successful
        if result < 0 {
            panic!("Socket configuration failed");
        }
        // ---------------------------
        // -- GET THE ADAPTER NAMES --
        // ---------------------------
        // get the display name
        let display_name: String = name.to_string();
        // get the adapter name
        let adapter_name: String = String::from_utf8_lossy(&name_buf).to_string();
        // ----------------------------------
        // -- CONSTRUCT THE ADAPTER OBJECT --
        // ----------------------------------
        // construct the file object
        let file: File = unsafe { File::from_raw_fd(fd) };
        // construct the adapter object
        let adapter: Adapter = Adapter {
            file: file,
            display_name: display_name,
            adapter_name: adapter_name,
        };
        // return the adapter object
        return adapter;
    }
}

// io implementation
impl Adapter {
    // Read from the adapter
    pub fn read(&mut self, buffer: &mut [u8]) -> usize {
        // read from the adapter
        let result: usize = self.file.read(buffer).unwrap();
        // return the result
        return result;
    }
    // Write to the adapter
    pub fn write(&mut self, buffer: &[u8]) -> usize {
        // write to the adapter
        let result: usize = self.file.write(buffer).unwrap();
        // return the result
        return result;
    }
}