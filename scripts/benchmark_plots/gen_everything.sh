#!/bin/sh

SCRIPT_FOLDER=$(dirname "$0")

mkdir -p "$SCRIPT_FOLDER/output/"

for f in "$SCRIPT_FOLDER/data/"*
do
    python $SCRIPT_FOLDER/gen_plot.py --in=$f --out=output/
done

