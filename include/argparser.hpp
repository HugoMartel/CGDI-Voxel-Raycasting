/**
 * @file argparser.hpp
 */
#ifndef __RAYCAST_ARGPARSER__
#define __RAYCAST_ARGPARSER__

#include <string>

#define BLOCK_SHAPES_FILE_PATH "../voxels/1_20_1_block_shapes.json"

/**
 * Enum storing all the possible algorithm types that can be used in the program.
 */
enum RayAlgorithms {
    SLABS   = 0,
    BITMASK = 1
};

/**
 * TODO
 */
std::ostream& operator<<(std::ostream& os, const RayAlgorithms& a);

/**
 * Class used to parse program arguments and store them.
 */
struct ArgParser {
public:
    // Attributes
    /**
     * JSON file containing a chunk content generated from a NBT file.
     * @note Can be generated using the script in this repository.
     */
    std::string chunkPath;
    /**
     * SON file containing all the AABB for all blocks
     * @note A default file is available in the `voxels/` folder.
     */
    std::string shapesPath;
    /**
     * Section of the chunk to extract.
     * @note Defaults to 0 if not provided.
     */
    int section;
    /**
     * Selects the ray shooting algorithm to use.
     */
    RayAlgorithms ray_algorithm;
    /**
     * Enables verbose output.
     */
    bool verbose;
    /**
     * Enables benchmark mode.
     */
    bool benchmark;

    // Constructors
    /**
     * Constructor taking the program input as an argument directly
     * @param   argc    Amount of arguments.
     * @param   argv    Array of strings of size argc.
     */
    ArgParser(const int argc, const char** argv);
};

#endif//__RAYCAST_ARGPARSER__

