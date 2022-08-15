#!/bin/bash -e

sudo apt-get update
sudo apt-get install -y gpg wget git python3-pip ccache p7zip-full g++-mingw-w64-x86-64 gcc-mingw-w64-x86-64 mingw-w64-tools cmake ninja-build

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
export VCPKG_ROOT="$PWD"
./bootstrap-vcpkg.sh
./vcpkg version

export VCPKG_DEFAULT_TRIPLET=x64-mingw-static
export VCPKG_DEFAULT_HOST_TRIPLET=x64-linux
export VCPKG_BINARY_SOURCES=files,"$HOME"/.vcpkg,readwrite
patch -Np1 -i ../cmake/graphicsmagic-mingw-w64.patch

./vcpkg install fmt zlib jbigkit graphicsmagick
cd ..

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -GNinja \
         -DSYSTEM_NBTP=OFF \
         -DCMAKE_INSTALL_PREFIX=/ \
         -DUSE_GRAPHICSMAGICK=ON \
         -DUSE_CCACHE=ON \
         -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}"/scripts/buildsystems/vcpkg.cmake \
         -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
         -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="$PWD"/../cmake/MinGWCross.cmake

export DESTDIR="$PWD"/dist
export ARTIFACTDIR="$PWD"/../package
mkdir -p "${ARTIFACTDIR}"
DESTDIR="${DESTDIR}" ninja install

cd dist/
ARCHIVE_NAME=minemap-win64
7za a -tzip "${ARTIFACTDIR}/${ARCHIVE_NAME}.zip" .
