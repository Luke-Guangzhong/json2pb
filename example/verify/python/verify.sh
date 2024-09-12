#!/bin/bash
protoc example.proto -I ../../proto  --python_out=./
python3 verify.py ../HEX_MSG.txt hex