/**
 * @file scene.cpp
 */
#include "scene.hpp"

#include <fstream>

#include <json/json.h>

#include "util.hpp"


SandboxScene::SandboxScene(
    const std::string& chunkFileName,
    const std::string& blocksFileName
) : voxels(16, std::vector<std::vector<Voxel>>(16, std::vector<Voxel>(16, Voxel()))) {
    // Checks if files exists
    std::ifstream chunkFile(chunkFileName, std::ios_base::in);
    std::ifstream blocksFile(blocksFileName, std::ios_base::in);

    // Loads JSON files
    Json::Value chunkData;
    Json::Value blocksData;
    chunkFile >> chunkData;
    blocksFile >> blocksData;

    // Get the JSON Array of Y sections
    const Json::Value sections = chunkData["sections"];

    //============================ 
    // ONLY KEEP section Y=3
    //============================ 
    int section_index = 0;
    while (sections[section_index++]["Y"] != 3);

    // Create the Voxel objects at correct coords
    // voxels.resize(16, std::vector<std::vector<Voxel>>());// Y
    for (int y=0; y<16; ++y) {
        // Get the block palette for the current Y
        const Json::Value palette = sections[section_index]["palette"];

        // Grid of 256 at height y
        // voxels[y].resize(16, std::vector<Voxel>());// Z

        // Get the AABB of all the blocks in the palette
        std::vector<std::vector<AABB>> palette_shapes(palette.size());
        for (unsigned int i=0; i<palette.size(); ++i) {
            const Json::Value block = blocksData[palette[i]["Name"].asString()];
            const std::string block_shape_str = block["states"][0]["shape"].asString();
            // TODO: change default shape to correct shape
            palette_shapes[i] = str_to_aabbvector(block_shape_str);
        }

        if (palette.size() == 1) {
            // No data for the section since the palette has only one block
            for (int z=0; z<16; ++z) {
                // Fill with the only possible Voxel
                for (int x=0; x<16; ++x) {
                    for (const AABB& box : palette_shapes[0])
                        voxels[y][z][x].addAABB(box);
                }
                // voxels[y][z].resize(16, Voxel(palette_shapes[0]));// X
            }
        } else {
            // Each block is mapped in the "data" part of the section
            for (int z=0; z<16; ++z) {
                // voxels[y][z].resize(16);// X
                for (int x=0; x<16; ++x) {
                    // Get the block type
                    std::cout << '('<<y<<','<<z<<','<<x<<")\n";
                    const int block_id = sections[section_index]["data"][y*16*16+z*16+x].asInt();
                    // Overwrite empty voxels
                    std::cout << '('<<y<<','<<z<<','<<x<<")\n";
                    for (const AABB& box : palette_shapes[block_id])
                        voxels[y][z][x].addAABB(box);
                }
            }
        }

        ++section_index;
    }
}

