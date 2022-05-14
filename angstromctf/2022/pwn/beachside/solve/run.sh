#!/bin/sh
set -e

rm -f out.so

cd example-helloworld/
npm run build:program-c

cd ..

mv ./example-helloworld/dist/program/helloworld.so ./out.so

python solve.py
