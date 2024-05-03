/**
 * @file argparser.hpp
 */
#ifndef __RAYCAST_ARGPARSER__
#define __RAYCAST_ARGPARSER__

#include <string>

/**
 * Default Block shapes file to used if none is provided.
 */
#define BLOCK_SHAPES_FILE_PATH "../voxels/1_20_1_block_shapes.json"

/**
 * Enum storing all the possible algorithm types that can be used in the program.
 */
enum RayAlgorithms {
    SLABS            = 0,
    SLABS_MARCHING   = 1,
    BITMASK          = 2,
    BITMASK_MARCHING = 3
};

/**
 * Stream write override for a RayAlgorithms enum item.
 * @param   os  Output stream to write to.
 * @param   a   RayAlgorithms to convert to string and write.
 * @return  The same output stream from the arguments.
 */
std::ostream& operator<<(std::ostream& os, const RayAlgorithms& a);
/**
 * Converts a RayAlgorithms enum item to string.
 * @param   a   RayAlgorithms to convert to string.
 * @return  Corresponding std::string.
 */
std::string convert_to_string(const RayAlgorithms& a);


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
     * Marching step value in case the marching algorithm is chosen.
     */
    double marching_step;
    /**
     * Enables verbose output.
     */
    bool verbose;
    /**
     * Enables benchmark mode.
     */
    bool benchmark;
    /**
     * Benchmark folder to output to.
     */
    std::string output_folder;

    // Constructors
    /**
     * Constructor taking the program input as an argument directly
     * @param   argc    Amount of arguments.
     * @param   argv    Array of strings of size argc.
     */
    ArgParser(const int argc, const char** argv);
};

#endif//__RAYCAST_ARGPARSER__

