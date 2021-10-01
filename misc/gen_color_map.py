from typing import List, Tuple
from PIL import Image

import re
import sys

LINE_MATCHER = re.compile('{(\d+), (\d+), (\d+), (\d+)}')


def usage():
    print('Usage:\n\tgen_color_map <input.txt> <output.gif>')


def run_colors(contents: str) -> Tuple[int, List[int]]:
    colors = []
    num_colors = 0
    for line in contents.splitlines():
        if not line or line.startswith('#'):
            continue
        matched = LINE_MATCHER.search(line)
        if not matched:
            raise ValueError(f'Malformed line: {line}')
        num_colors += 1
        for i in range(1, 5):
            colors.append(int(matched.group(i)))
    return (num_colors, colors)


def main():
    if len(sys.argv) != 3:
        usage()
        sys.exit(1)
    input_filename = sys.argv[1]
    output_filename = sys.argv[2]
    with open(input_filename, 'rt') as f:
        contents = f.read()
    num_colors, colors = run_colors(contents)
    print(f'Number of colors in {input_filename}: {num_colors}')
    im = Image.frombytes('RGBA', (4, num_colors // 4), bytes(colors))
    im.save(open(output_filename, 'wb'), format='GIF')


if __name__ == '__main__':
    main()
