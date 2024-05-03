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
$SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/test_world_chunk.json -s 4 --algorithm slabs --benchmark -o $SCRIPT_DIR/benchmark_plots/data/
$SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm slabs --benchmark -o $SCRIPT_DIR/benchmark_plots/data/

# Marching slabs
echo "Benchmarking marching slabs"
for step in $(LANG=en-US seq 0.01 0.01 0.5)
do
    echo "step=$step"
    $SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/test_world_chunk.json -s 4 --algorithm slabs_marching --step $step --benchmark -o $SCRIPT_DIR/benchmark_plots/data/
    $SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm slabs_marching --step $step --benchmark -o $SCRIPT_DIR/benchmark_plots/data/
done

# Bitmask
echo "Benchmarking bitmask"
$SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/test_world_chunk.json -s 4 --algorithm bitmask --benchmark -o $SCRIPT_DIR/benchmark_plots/data/
$SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm bitmask --benchmark -o $SCRIPT_DIR/benchmark_plots/data/

# Marching slabs
echo "Benchmarking marching bitmask"
for step in $(LANG=en-US seq 0.01 0.01 0.5)
do
    echo "step=$step"
    $SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/test_world_chunk.json -s 4 --algorithm bitmask_marching --step $step --benchmark -o $SCRIPT_DIR/benchmark_plots/data/
    $SCRIPT_DIR/../build/raycast -c $SCRIPT_DIR/../voxels/superflat_sandstone_chunk.json -s 3 --algorithm bitmask_marching --step $step --benchmark -o $SCRIPT_DIR/benchmark_plots/data/
done

