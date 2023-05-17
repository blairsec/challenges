#!/bin/sh
tar -hczvf dist.tar.gz --exclude='server/target' --exclude='chall/target' server chall solve.py Dockerfile sailors_revenge.so
