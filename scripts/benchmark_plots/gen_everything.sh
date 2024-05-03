#!/bin/sh

SCRIPT_FOLDER=$(dirname "$0")
N="10000"

mkdir -p "$SCRIPT_FOLDER/output/"

# Plot merging all the data for a given algorithm
echo "Generating plots comparing slabs and marching slabs"
echo "> Test world chunk"
python "$SCRIPT_FOLDER/compare_slabs.py" --prefix "$SCRIPT_FOLDER/data/benchmark_test_world_chunk_"$N"_slabs" --out "$SCRIPT_FOLDER/output/"
python "$SCRIPT_FOLDER/compare_slabs.py" --prefix "$SCRIPT_FOLDER/data/benchmark_test_world_chunk_"$N"_bitmask" --out "$SCRIPT_FOLDER/output/"

echo "> Superflat sandstone chunk"
python "$SCRIPT_FOLDER/compare_slabs.py" --prefix "$SCRIPT_FOLDER/data/benchmark_superflat_sandstone_chunk_"$N"_slabs" --out "$SCRIPT_FOLDER/output/"
python "$SCRIPT_FOLDER/compare_slabs.py" --prefix "$SCRIPT_FOLDER/data/benchmark_superflat_sandstone_chunk_"$N"_bitmask" --out "$SCRIPT_FOLDER/output/"

# Single file plots
for f in "$SCRIPT_FOLDER/data/"*
do
    echo "Generating plots for $f"
    python "$SCRIPT_FOLDER/basic_plots.py" --in "$f" --out "$SCRIPT_FOLDER/output/" > "$SCRIPT_FOLDER/output/$(basename "$f" .txt)-output.txt"
done

