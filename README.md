# CGDI Voxel Raycasting

Project for the CGDI course at ENS de Lyon.

## Building

Dependencies to install:
- jsoncpp
- cmake

On Debian like:
`apt install cmake libjsoncpp-dev`

On Fedora like:
`dnf install cmake jsoncpp-devel`


Building the project:
```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

## Usage

**Examples:**

Starting the Polyscope GUI to visualize ray shooting algorithms computation steps:

```bash
./raycast -c ../voxels/superflat_sandstone_chunk.json
```

Generating a benchmark file:
```bash
./raycast -c ../voxels/test_world_chunk.json -s 4 --algorithm slabs --benchmark --verbose
```

Program arguments:

**required**
* `--chunk <file path>`, `-c <file path>`: JSON file containing a chunk content generated from a NBT file (can be generated using the script in this repository).

**optional**
* `--blockshapes <file path>`, `-b <file path>`: JSON file containing all the AABB for all blocks a default one is available in the `voxels/` folder.

* `--section <section number>`, `-s <section number>`: Section of the chunk to extract, defaults to 0 if not provided.

* `--algorithm <algorithm name>`, `-a <algorithm name>`: Selects the algorithm to use for the ray shooting. Algorithm name can be:
    - slabs *(DEFAULT)*
    - marching
    - bitmask

* `--verbose`, `-v`: Enables verbose output.

* `--benchmark`: Enables benchmark mode.

## Scripts

Various scripts are available to generate benchmark plots or extract voxel data from Minecraft world region files in the `scripts/` folder.

### MCA to JSON

Use this script to extract chunk blocks from a Minecraft .mca region file from a world folder.

**Example:**

Extraction of the spawn chunks' blocks of a given minecraft world.
```bash
./mca_to_json.sh regions/test_world.mca
```

### Benchmark plots

Use this script to generate plots from generated text files from the main program using the `--benchmark` argument.

**Example:**

Generates all the plots and extracts data from all the benchmark files in the data/ folder.
```bash
./gen_everything.sh
```

