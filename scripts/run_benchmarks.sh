#!/bin/bash

SCRIPT_DIR=$(dirname "$0")

# What is run:
# - Every algorithm
# - Two scenes:
#   * flat ground and 2 beds
#   * valley with some smaller cubes
# - Marching steps between 0.01:0.5:0.01

# Slabs
echo "Benchmarking slabs"
$SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/test_world_chunk.json -s 4 --algorithm slabs --benchmark
$SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm slabs --benchmark

# Marching slabs
echo "Benchmarking marching slabs"
for step in $(LANG=en-US seq 0.01 0.01 0.5)
do
    echo "step=$step"
    $SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/test_world_chunk.json -s 4 --algorithm marching --step $step --benchmark
    $SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm marching --step $step --benchmark
done

# Bitmask
# echo "Benchmarking bitmask"
# $(SCRIPT_DIR)/../build/raycast -c $(SCRIPT_DIR)/../voxels/test_world_chunk.json -s 4 --algorithm bitmask --benchmark
# $(SCRIPT_DIR)/../build/raycast -c $(SCRIPT_DIR)/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm bitmask --benchmark

# Move all the benchmarks to the data folder for plot generation
mv *.txt "$SCRIPT_DIR/benchmark_plots/data/"

