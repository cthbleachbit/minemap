#!/bin/sh
# MinGW64 only - do not use on *nix systems
# Copy dependency DLLs into installation directory and prepare for release

# $1 = Build Dir
# $2 = Install Prefix
# $3 = Version number

set -e

DLLS=`ldd "$1/src/libminemap.dll" "$1/src/minemap.exe" "$1/src/pamenim.exe" | grep /mingw64 | cut -d ' ' -f 3 | sort | uniq`
cp -v $DLLS "$2/minemap-$3"/
find "$2/minemap-$3" -name "*.a" -delete
find "$2/minemap-$3" -name "*.dll" -exec strip {} \;
