#!/bin/zsh
UID=$(($RANDOM+1024))

timeout 60s python3 ctf/server.py
