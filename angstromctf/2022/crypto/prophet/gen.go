package main

import (
	"encoding/binary"
	"fmt"
	"math/rand"
)

func main() {
	flag := "actf{i_c4n_f0rs33_th3_p4s7_t00_}"
	rand.Seed(6279737917174324099)
	// drastically slow down naive brute force
	for i := 0; i < 100000; i += 1 {
		rand.Uint64()
	}
	for i := 0; i < 4; i += 1 {
		fmt.Printf("flag chunk: %d\n", binary.LittleEndian.Uint64([]byte(flag)[i*8:i*8+8])^rand.Uint64())
	}
	gap := 0
	for i := 0; i < 607; i += 1 {
		fmt.Println(rand.Uint64())
		for j := 0; j < gap; j += 1 {
			rand.Uint64()
		}
		gap = (gap + 1) % 13
	}
}
