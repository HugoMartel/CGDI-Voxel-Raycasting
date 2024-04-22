#!/bin/bash

# Args parsing
if [ $# -eq 0 ]
  then
    echo "Missing .mca file argument"
    exit
fi
if [ ${1: -4} != ".mca" ]; then
    echo "Non .mca file provided"
    exit
fi

# Build the MCA2NBT project
echo "> Setup"
cd MCA2NBT
make > /dev/null # Build the executable from c sources and suppress output
#WARNING: If this step fails remove the `> /dev/null` to be able to debug the problem
cd ..

# Create the output files folder
GEN_FOLDER=generated
mkdir -p $GEN_FOLDER

# Region MCA TO NBT Chunks
echo "> MCA -> NBTs"
NBT_FOLDER="$GEN_FOLDER/$(basename $1 .mca)_nbt"
NBT_FOLDER_TMP=${1::-4} # Remove the .mca from input file
chmod 777 $1 # Necessary otherwise, mca2nbt can't access the archive
mkdir -p $NBT_FOLDER_TMP # Create the  output folder with the correct permissions
chmod 777 $NBT_FOLDER_TMP
./MCA2NBT/mca2nbt $1 # Convert the .mca region archive to .nbt chunk files
mv -fu $NBT_FOLDER_TMP/* $NBT_FOLDER/ # Move the nbt files to the correct location
chmod 755 $NBT_FOLDER # Set the correct permissions for the final .nbt files

# NBT Chunks to Blocks JSON
echo "> NBTs -> JSONs"
NBT_FILES=$NBT_FOLDER/*
JSON_FOLDER="$GEN_FOLDER/$(basename $1 .mca)_json"
mkdir -p $JSON_FOLDER

# For each chunk .nbt file 
for f in $NBT_FILES
do
    NBT=$(basename $f)

    if [ ${NBT: -4} == ".nbt" ]; then
        OUT_NAME="$JSON_FOLDER/$(basename $f .nbt).json"
        # echo "$f -> $OUT_NAME"
        python nbt_to_json.py --in=$f --out=$OUT_NAME
    fi
done
echo "Output JSON files written in $JSON_FOLDER/"

