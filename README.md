# CGDI Voxel Raycasting

Project for the CGDI course at ENS de Lyon.

## Building

Dependencies to install:
- jsoncpp
- cmake

Building the project:
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

## Usage

Example:
```
./raycast ../voxels/superflat_sandstone_chunk.json
```

