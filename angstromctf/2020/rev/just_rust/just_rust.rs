use std::io;
use std::process::exit;

fn print_state(state: [[u8; 8]; 8]) {
    for i in 0..8 {
        for j in 0..8 {
            let dchar: char = state[i][j] as char;
            print!("{}", dchar)
        }
        println!();
    }
}

fn main() {
    println!("What do you want to encode?");
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Cannot read input.");
    input = input.trim().to_string();
    if input.len() != 32 {
        println!("Input must be 32 characters.");
        exit(1);
    }
    let mut state: [[u8; 8]; 8] = [[b'\x40'; 8]; 8];
    for (i, chr) in input.chars().enumerate() {
        let bit: u8 = (i as u8) / 8;
        let mut vals = [b'\x00'; 2];
        chr.encode_utf8(&mut vals);
        for j in 0..8 {
            let desired: u8 = (vals[0] & (1 << j)) >> j;
            state[j][(i + j) % 8] |= (desired << bit) as u8;
        }
    }
    print_state(state);
}