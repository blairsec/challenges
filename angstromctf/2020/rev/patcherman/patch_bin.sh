#!/bin/bash
printf '\x00\x00\x00\x00\x00\x00\x00\x00' | dd of=patcherman bs=1 seek=40 conv=notrunc
