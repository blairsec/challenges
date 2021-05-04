#!/bin/zsh
UID=$(($RANDOM+1024))
timeout 3600s python3 ctf/server.py
