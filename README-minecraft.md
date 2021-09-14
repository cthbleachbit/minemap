Map format as of 1.10.2
=======================

NBT
---

In side a `world/data/map_xxx.dat` file:

```
0a      0000
# The root tag, a 0x0a compound tag with name length 0000 i.e. no name
        0a      0004    data
                01      0005    scale        00
#               ^ byte
                01      0009    dimension    01
#                                            ^ The End dimension
                02      0006    height       0080
#               ^ short                      ^ 128
                02      0005    width        0080
                03      0007    xCenter      05f5e100
#               ^ int                        ^ 1e8
                03      0007    zCenter      05f5e100
                07      0006    colors       00004000
#               ^ byte array                 ^ byte length: 16384
                ... (tons of bytes representing pixels)
        00
#       ^ End of data compound tag
00
# End of root tag
```

Each byte C representing a pixel is cacluated from the base color B and variant (0-3) number V. When V is 0, 1, 2, 3, the color RGB is multiplied by 180, 220, 255, 135 respectively then divided by 255 and floor to an integer RGB.

Changes in version 1.12
-----------------------

* `width` and `height` are removed from the tag, and maps must have a fixed 128x128 size
* Added byte `trackingPosition`
* Added byte `unlimitedTracking`
* Added byte `locked`
* Added list of compounds `banners`
* Added list of compounds `frames`
* Added byte `dimension`
* Added 64 colors for a total of 208 colors

Changes in version 1.16
-----------------------
* Added 28 colors for a total of 236 colors

Changes in version 1.17
-----------------------
* `dimension` is now a namespaced string instead of a byte
* Added 12 colors for a total of 248 colors