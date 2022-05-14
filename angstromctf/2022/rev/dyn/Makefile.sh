#!/bin/sh
# not actually a Makefile, but naming it similarly sounds cool idk

# compiled with:
# rustc 1.60.0 (7737e0b5c 2022-04-04)
# binary: rustc
# commit-hash: 7737e0b5c4103216d6fd8cf941b7ab9bdbaace7c
# commit-date: 2022-04-04
# host: x86_64-unknown-linux-gnu
# release: 1.60.0
# LLVM version: 14.0.0 
rustc -C target-feature=+crt-static -C opt-level=z --target=x86_64-unknown-linux-musl main.rs
