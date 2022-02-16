from typing import List
from gen_color_map import read_color_map

import sys


def generate_color_book_header(prefix: str):
    return f"""GIMP Palette
Name: {prefix}
Columns: 4
#
"""


def generate_color_book_inner(data: List[int]) -> str:
    output = ''
    index = 0
    for i in range(16, len(data), 4):
        tuples = data[i:i+3]  # skip the 4th column
        formatted = ["{:>3}".format(str(j)) for j in tuples]
        output += f"{' '.join(formatted)} #{index}\n"
        index += 1
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
