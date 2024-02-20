import argparse
from pathlib import Path
from xml.etree.ElementTree import parse


def main():
    parser = argparse.ArgumentParser("ROM-upload")
    parser.add_argument('circuit')
    parser.add_argument('rom')
    parser.add_argument('-o', '--out')
    args = parser.parse_args()
    args.out = args.out if args.out else 'a.circ'

    tree = parse(args.circuit)
    root = tree.getroot()

    with open(args.rom) as f:
        rom_data = [line.strip() for line in f.readlines()]

    rom_header = ["addr/data: 24 8"]
    rom_body = [f"{line[0:2]} {line[2:4]} {line[4:6]} {line[6:8]}" for line in rom_data[1:]]

    root.find("./circuit/comp/a[@name='contents']").text = "\n".join(rom_header+rom_body)
    tree.write(args.out)
    print("ROM loaded")


if __name__ == "__main__":
    main()
