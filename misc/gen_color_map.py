from typing import List

import sys
import os
import re
from typing import NamedTuple, Tuple

PALETTE_FILE_PATTERN = r"rgb-base-(\d.\d+)\.txt"


class NamedRGBA(NamedTuple):
    name: str
    color: Tuple[int, int, int, int]

    def __repr__(self):
        return f"NamedRGBA<{self.name}, {self.color}>"

    def as_c_array_repr(self):
        return f"{self.color[0]}, {self.color[1]}, {self.color[2]}, {self.color[3]}"


def parse_color_base_entry(line: int, content: str) -> NamedRGBA:
    # Every line should be of form "rgb-integer base_id color_name"
    parts = content.split()
    if len(parts) < 2:
        raise ValueError(f"Invalid content on line {line}: {content}")
    rgb = int(parts[0], 0)
    color_id = int(parts[1], 0)
    color_name = parts[2] if len(parts) == 3 else f"base id {color_id}"
    r = rgb >> 16
    g = rgb >> 8 & 0x000000ff
    b = rgb & 0x000000ff
    a = 0 if color_id == 0 else 0xff
    c = NamedRGBA(color_name, (r, g, b, a))
    print(f"Found color {c}")
    return c


def derive_brightness(base: NamedRGBA, brightness: int) -> NamedRGBA:
    """
    Derive brightness from base color and 4 level of brightness
    :param base: base color in RGBA, each component from 0 to 255
    :param brightness: from 0 to 4
    :return: resulting color at index (4*base_id + brightness)
    """
    if brightness < 0 or brightness > 3:
        raise ValueError(f"Incorrect brightness {brightness}")

    coeff = 180 if brightness == 0 else 220 if brightness == 1 else 255 if brightness == 2 else 135
    base_color = base.color
    r = int(base_color[0] * coeff / 255)
    g = int(base_color[1] * coeff / 255)
    b = int(base_color[2] * coeff / 255)
    return NamedRGBA(f"{base.name}_{coeff}", (r, g, b, base_color[3]))


def parse_color_map(content: str) -> List[NamedRGBA]:
    lineno = 0
    data = []
    for line in content.splitlines():
        lineno += 1
        line = line.strip()
        if line.startswith('#') or not line:
            continue
        base = parse_color_base_entry(lineno, line)
        for i in [0, 1, 2, 3]:
            derived = derive_brightness(base, i)
            data.append(derived)
    if len(data) % 4 != 0:
        raise ValueError(f'Color palette width must be a multiple of 4!')
    return data


def read_color_map(filename: str) -> List[NamedRGBA]:
    with open(filename, 'rt') as f:
        content = f.read()
    return parse_color_map(content)


def generate_header_inner(input_name: str, identifier: str, colors: List[NamedRGBA]) -> str:
    # height = # of base colors
    height: int = len(colors) // 4;
    # array size = # of colors * 4 (for RGBA)
    array_size: int = len(colors) * 4;
    return f"""
# 0 "{input_name}"
const size_t {identifier}_HEIGHT = {height};
# 1 "{input_name}"
const uint8_t {identifier}_DATA[{array_size}] = {{ {', '.join([i.as_c_array_repr() for i in colors])} }};
// safe-guards
static_assert({identifier}_HEIGHT > 0, "{identifier}_HEIGHT must be greater than zero!");
"""


def generate_header() -> str:
    return """// Automatically generated. DO NOT EDIT.
// To create maps for new versions, create a new table in /misc directory.
#pragma once 
#include <cstddef>
#include <cstdint>
"""


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <INPUT...> <OUTPUT>")
        return
    input_files = sys.argv[1:-1]
    output_file = sys.argv[-1]
    headers = generate_header()
    for input_file in input_files:
        print(f"Generating color values for {input_file}...")
        colors = read_color_map(input_file)
        path = os.path.abspath(input_file)
        basename = os.path.basename(input_file)
        mc_version_match = re.search(PALETTE_FILE_PATTERN, basename)
        if mc_version_match is None:
            raise ValueError(f'Unknown palette filename: {input_file}')
        mc_version = mc_version_match.group(1)
        identifier = f"MC_{mc_version.replace('.', '_')}"
        headers += generate_header_inner(path, identifier, colors)
    print(f"Writing {output_file}...")
    with open(output_file, 'wt') as f:
        f.write(headers)


if __name__ == '__main__':
    main()
