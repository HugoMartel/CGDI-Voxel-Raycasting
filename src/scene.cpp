/**
 * @file scene.cpp
 */
#include "scene.hpp"

#include <fstream>
#include <cassert>

#include <json/json.h>

#include "util.hpp"
#include "geometry.hpp"


SandboxScene::SandboxScene(const std::string& chunkPath, const std::string& shapesPath,
                           const int chosen_section)
: voxels(CHUNK_SIDE_SIZE, std::vector<std::vector<Voxel>>(
            CHUNK_SIDE_SIZE, std::vector<Voxel>(
                CHUNK_SIDE_SIZE, Voxel()))) {
    // Checks if files exists
    std::ifstream chunkFile(chunkPath, std::ios_base::in);
    std::ifstream shapesFile(shapesPath, std::ios_base::in);

    // Loads JSON files
    Json::Value chunkData;
    Json::Value shapesData;
    chunkFile >> chunkData;
    shapesFile >> shapesData;

    // Get the JSON Array of Y sections
    const Json::Value sections = chunkData["sections"];

    //============================
    // ONLY KEEP section Y=3
    //============================
    int section_index = 0;
    while (sections[section_index]["Y"] != chosen_section)
        ++section_index;

    // Get the block palette for the current Y
    const Json::Value palette = sections[section_index]["palette"];

    // Grid of 256 at height y
    // voxels[y].resize(16, std::vector<Voxel>());// Z

    // Get the AABB of all the blocks in the palette
    std::cout << "PALETTE\n";//! DEBUG
    std::vector<std::vector<AABB>> palette_shapes(palette.size(), std::vector<AABB>());
    for (unsigned int i=0; i<palette.size(); ++i) {
        std::cout << palette[i] << '\n';//! DEBUG
        // Find the block in the block shapes JSON corresponding to
        // the current block in the palette
        const Json::Value block = shapesData[palette[i]["Name"].asString()];
        std::string block_shape_str = "";

        // If the block has properties, look for the correct shape in the block shapes JSON
        if (palette[i].isMember("Properties")) {
            const Json::Value block_properties = palette[i]["Properties"];
        // std::cout << palette[i]["Properties"] << '\n';//! DEBUG
        // std::cout << "STATES AMOUNT: " << block["states"].size() << '\n';//! DEBUG
            for (unsigned int state_i=0; state_i<block["states"].size(); ++state_i) {
                // std::cout << "STATE: " << state_i << '\n';//! DEBUG
                // std::cout << block["states"][state_i]["properties"] << '\n';//! DEBUG
                if (block["states"][state_i]["properties"] == block_properties) {
                    block_shape_str = block["states"][state_i]["shape"].asString();
                    break;
                }
            }
        } else {
            // Otherwise, simply select the first shape since it is the only choice
            block_shape_str = block["states"][0]["shape"].asString();
        }

        assert(block_shape_str != "");
        // Convert the AABB string to a vector of AABB and fill in the palette shapes
        palette_shapes[i] = str_to_aabbvector(block_shape_str);
        //! DEBUG
        for (size_t j=0; j<palette_shapes[i].size(); ++j) {
            std::cout << palette_shapes[i][j] << '\n';
        }
    }

    // Create the Voxel objects at correct coords
    // voxels.resize(16, std::vector<std::vector<Voxel>>());// Y
    if (palette.size() == 1) {
        // No data for the section since the palette has only one block
        for (int y=0; y<16; ++y) {
            for (int z=0; z<16; ++z) {
                // Fill with the only possible Voxel
                for (int x=0; x<16; ++x) {
                    for (const AABB& box : palette_shapes[0]) {
                        voxels[y][z][x].addAABB(box);
                    }
                }
                // voxels[y][z].resize(16, Voxel(palette_shapes[0]));// X
            }
        }
    } else {
        // Each block is mapped in the "data" part of the section
        for (int y=0; y<16; ++y) {
            for (int z=0; z<16; ++z) {
                // voxels[y][z].resize(16);// X
                for (int x=0; x<16; ++x) {
                    // Get the block type
                    const int block_id = sections[section_index]["data"][y*16*16+z*16+x].asInt();
                    // Overwrite empty voxels
                    for (const AABB& box : palette_shapes[block_id]) {
                        voxels[y][z][x].addAABB(box);
                    }
                }
            }
        }
    }
}
