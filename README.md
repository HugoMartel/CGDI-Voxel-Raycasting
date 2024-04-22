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
./raycast -c ../voxels/superflat_sandstone_chunk.json
```

Program arguments:

**required**
* `--chunk <file path>`, `-c <file path>`: JSON file containing a chunk content generated from a NBT file (can be generated using the script in this repository).

**optional**
* `--blockshapes <file path>`, `-b <file path>`: JSON file containing all the AABB for all blocks a default one is available in the `voxels/` folder.

* `--section <section number>`, `-s <section number>`: Section of the chunk to extract, defaults to 0 if not provided.

* `--verbose`, `-v`: Enables verbose output.

