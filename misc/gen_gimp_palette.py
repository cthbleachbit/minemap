from typing import List
from gen_color_map import read_color_map, NamedRGBA

import sys


def generate_color_book_header(prefix: str):
    return f"""GIMP Palette
Name: {prefix}
Columns: 4
#
"""


def generate_color_book_inner(data: List[NamedRGBA]) -> str:
    output = ''
    for i in range(4, len(data)):
        named_color = data[i]
        tuples = (named_color.color[0], named_color.color[1], named_color.color[2])
        formatted = ["{:>3}".format(str(j)) for j in tuples]
        output += f"{' '.join(formatted)} #{named_color.name}\n"
    return output


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <INPUT> <OUTPUT_PREFIX>")
        print(f"Note: Indices 0-3 are reserved for transparency and excluded from the palette")
        return
    input_file = sys.argv[1]
    output_prefix = sys.argv[2]
    output_file = output_prefix + '.gpl'
    data = read_color_map(input_file)
    color_book = generate_color_book_inner(data)
    with open(output_file, 'wt') as f:
        f.write(generate_color_book_header(output_prefix))
        f.write(color_book)


if __name__ == "__main__":
    main()
