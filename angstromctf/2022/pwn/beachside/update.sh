#!/bin/sh

cd programs && make build
cd ..

tar czvf beachside.tar.gz --exclude='./beachside/target/*' ./beachside
