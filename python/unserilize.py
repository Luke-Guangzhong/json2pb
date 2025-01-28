#!/usr/bin/python3
#####################################################
#                                                   #
#      Tools for parsing protobuf information       #
#                                                   #
#####################################################
# **NOTE**
# Using (shell cmd) $ ./run.sh;
# to generate *_pb2.py if you updated the *.proto file
#
import sys
import device_pb2
import binascii
import operator

def analysis_hex(file_path, msg):
    data = []

    with open(file_path, "r") as f:
        for data_line in f.readlines():
            if data_line != None:
                data.append(data_line.strip())

    data_str = "".join(data)

    content = binascii.a2b_hex(data_str)

    msg.ParseFromString(content)

    print("\033[1;32;40m-------------------------- \033[0m")
    print(msg)
    print("\033[1;32;40m-------------------------- \033[0m")


def main():
    msg = device_pb2.Device()
    analysis_hex(file_path="/home/luke/project/json2pb/tmp/pb.hex", msg=msg)


if __name__ == "__main__":
    main()
