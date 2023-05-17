#!/bin/bash
set -e

cd gaga0
python3 ../../../gke-deploy/main.py
cd ../gaga1
python3 ../../../gke-deploy/main.py
cd ../gaga2
python3 ../../../gke-deploy/main.py
