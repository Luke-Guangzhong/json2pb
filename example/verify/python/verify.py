from example_pb2 import Device,example
import sys
import operator
import binascii


def analysis_bin(file_path, msg):
    msg = example()
    with open(file_path, "rb") as f:
        msg.ParseFromString(f.read())

    print(msg)


def analysis_hex(file_path):
    data = []

    with open(file_path, "r") as f:
        for data_line in f.readlines():
            if data_line != None:
                data.append(data_line.strip())

    data_str = "".join(data)

    content = binascii.a2b_hex(data_str)

    msg = example()

    msg.ParseFromString(content)

    print(msg)


def main():
    if len(sys.argv) != 3:
        print("\033[1;37;44m usage: <in file> <HEX/BIN> \033[0m")
        sys.exit(2)
    path = "."
    raw_file = sys.argv[1]
    file_type = sys.argv[2]

    if operator.eq(file_type.lower(), "hex"):
        analysis_hex(file_path=path + "/" + raw_file)
    elif operator.eq(file_type.lower(), "bin"):
        analysis_bin(file_path=path + "/" + raw_file)
    else:
        print("\033[1;37;44m usage: <in file> <HEX/BIN> \033[0m")
        print("\033[5;31;44m Please use <HEX/BIN> file type!!! \033[0m")


if __name__ == "__main__":
    main()
