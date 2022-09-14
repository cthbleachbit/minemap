#!/usr/bin/env python3
import json
import os
import subprocess as sp
import sys
from typing import Dict

import pynbtp


def run(minemap_bin: str, test_info: Dict[str, object]) -> int:
    arg_list = [minemap_bin,
                "-i", os.path.abspath(test_info["input_image"]),
                "-o", os.path.abspath(test_info["output_nbt"]),
                "-e", "/tmp/export.png",
                "--no-gz", "-g",
                "1.17"]
    if "replace" in test_info.keys():
        arg_list.append("--replace")
    print("Command line: ")
    print(" ".join(arg_list))
    result = sp.run(arg_list)
    return result.returncode


def cleanup(test_info: Dict[str, object]) -> None:
    os.remove(str(test_info["output_nbt"]))


def __main__():
    minemap_bin = sys.argv[1]
    test_info = {}
    with open(sys.argv[2], "r") as f:
        test_info = json.load(f)
    if run(minemap_bin, test_info) != 0:
        print("Exec FAIL: %s" % test_info["input_image"])
        return
    f = open(test_info["output_nbt"], "rb")
    try:
        tag, size = pynbtp.parseRoot(f)
    except RuntimeError:
        print("Parse Fail: %s" % test_info["output_nbt"])
        return
    data_tag = tag.lookup("data")
    if data_tag is None:
        print("Malformed /data: %s" % test_info["output_nbt"])
        return
    colors_tag = data_tag.lookup("colors")
    if colors_tag is None:
        print("Malformed /data/colors: %s" % test_info["output_nbt"])
        return
    colors = colors_tag.getPayload()
    for i in range(0, len(test_info["expect_color_codes"])):
        expect = test_info["expect_color_codes"][i]
        got = colors[i].getPayload()
        if expect != got:
            print("Color code mismatch at index %d: Expect %d, got %d" % (i, expect, got))
            return
    cleanup(test_info)
    return


if __name__ == "__main__":
    __main__()
