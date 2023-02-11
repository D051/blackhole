// Copyright (C) 2023 Dominik Schweigler

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

extern crate blackhole;

#[cfg(test)]
mod tests {

    #[test]
    fn adapter_test() {
        // create a new adapter
        let mut adapter: blackhole::Adapter = blackhole::Adapter::new("test");
        // verbose output
        adapter.print();
        
        let mut buffer: [u8; 1500] = [0; 1500];
        let i: i32 = 0;
        for i in 0..10 {
            // read from the adapter
            let result: usize = adapter.read(&mut buffer);
            // check if the read was successful
            if result < 0 {
                panic!("Read failed");
            }
            // print the read data
            println!("Read {} bytes", result);
        }
    }

}
