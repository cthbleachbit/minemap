from typing import List

import sys
import os
import re

PALETTE_FILE_PATTERN = r"rgba-(\d.\d+)\.txt"


def parse_color_map(content: str) -> List[int]:
    lineno = 0
    data = []
    for line in content.splitlines():
        lineno += 1
        line = line.strip()
        if line.startswith('#') or not line:
            continue
        if not line.startswith('{') or not line.endswith('},'):
            raise ValueError(f'Invalid content on line {lineno}: {line}')
        tuples = line[1:-2].split(', ')
        if len(tuples) != 4:
            raise ValueError(
                f'Expecting 4 values, but got {len(tuples)} on line {lineno}: {line}')
        num_tuples = []
        for value in tuples:
            try:
                v = int(value)
                if v < 0 or v >= 256:
                    raise ValueError(f'Value out of range: {value}')
                num_tuples.append(v)
            except ValueError as ex:
                raise ValueError(
                    f'Invalid value "{value}" on line {lineno}: {line}') from ex
        data.extend(num_tuples)
    if len(data) % 16 != 0:
        raise ValueError(f'Color palette width must be a multiple of 4!')
    return data


def read_color_map(filename: str) -> List[int]:
    with open(filename, 'rt') as f:
        content = f.read()
    return parse_color_map(content)


def generate_header_inner(input_name: str, identifier: str, data: List[int]) -> str:
    return f"""
# 0 "{input_name}"
const size_t {identifier}_HEIGHT = {len(data) // 16};
# 1 "{input_name}"
const uint8_t {identifier}_DATA[{len(data)}] = {{ {', '.join([str(i) for i in data])} }};
// safe-guards
static_assert({identifier}_HEIGHT > 0, "{identifier}_HEIGHT must be greater than zero!");
"""


def generate_header() -> str:
    return """// Automatically generated. DO NOT EDIT.
#pragma once
#include <stddef.h>
#include <stdint.h>
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
        data = read_color_map(input_file)
        path = os.path.abspath(input_file)
        basename = os.path.basename(input_file)
        mc_version_match = re.search(PALETTE_FILE_PATTERN, basename)
        if mc_version_match is None:
            raise ValueError(f'Unknown palette filename: {input_file}')
        mc_version = mc_version_match.group(1)
        identifier = f"MC_{mc_version.replace('.', '_')}"
        headers += generate_header_inner(path, identifier, data)
    print(f"Writing {output_file}...")
    with open(output_file, 'wt') as f:
        f.write(headers)


if __name__ == '__main__':
    main()
