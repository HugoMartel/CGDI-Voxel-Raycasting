/**
 * @file argparser.cpp
 */
#include "argparser.hpp"

#include <iostream>
#include <filesystem>
#include <cassert>
#include <cstring>
#include <array>

/**
 * Lookup table used to convert a RayAlgorithms enum item to string.
 */
std::array<std::string, 3> ray_algorithms_lookup({
    "slabs",
    "marching",
    "bitmask"
});

std::ostream& operator<<(std::ostream& os, const RayAlgorithms& a) {
    os << ray_algorithms_lookup[a];
    return os;
}

std::string convert_to_string(const RayAlgorithms& a) {
    return ray_algorithms_lookup[a];
}


ArgParser::ArgParser(const int argc, const char** argv)
: chunkPath(""), shapesPath(BLOCK_SHAPES_FILE_PATH), section(0),
  ray_algorithm(RayAlgorithms::SLABS), marching_step(0.1), verbose(false), benchmark(false) {
    // Iterate on the arguments
    for (int i=1; i<argc; ++i) {
        if (!std::strcmp(argv[i], "--verbose")) {
            // --verbose
            verbose = true;
        } else if (!std::strcmp(argv[i], "--benchmark")) {
            // --benchmark
            benchmark = true;
        } else if (!std::strcmp(argv[i], "--chunk") || !std::strcmp(argv[i], "-c")) {
            // --chunk
            if (i+1 == argc) {
                std::cout << "Missing chunk file path after the --chunk,-c argument\n";
                exit(-1);
            } else if (argv[i+1][0] == '-') {
                std::cout << "Missing chunk file path after the --chunk,-c argument\n";
                exit(-1);
            }
            chunkPath = argv[i+1];
            ++i;
        } else if (!std::strcmp(argv[i], "--shapes") || !std::strcmp(argv[i], "-b")) {
            // --blockshapes
            if (i+1 == argc) {
                std::cout << "Missing block shapes file path after the --blockshapes,-b argument\n";
                exit(-1);
            } else if (argv[i+1][0] == '-') {
                std::cout << "Missing block shapes file path after the --blockshapes,-b argument\n";
                exit(-1);
            }
            shapesPath = argv[i+1];
            ++i;
        } else if (!std::strcmp(argv[i], "--section") || !std::strcmp(argv[i], "-s")) {
            // --section
            if (i+1 == argc) {
                std::cout << "Missing block shapes file path after the --section,-s argument\n";
                exit(-1);
            }
            try {
                section = std::stoi(argv[i+1]);
            } catch (std::invalid_argument const& e) {
                std::cout << "Bad argument provided to --section,-s. Please provide an integer.\n";
                exit(-1);
            }
            ++i;
        } else if (!std::strcmp(argv[i], "--algorithm") || !std::strcmp(argv[i], "-a")) {
            // --algorithm
            if (i+1 == argc) {
                std::cout << "Missing algorithm name after the --algorithm,-a argument\n";
                exit(-1);
            } else if (argv[i+1][0] == '-') {
                std::cout << "Missing algorithm name after the --algorithm,-a argument\n";
                exit(-1);
            }
            if (!strcmp(argv[i+1], "slabs"))
                ray_algorithm = RayAlgorithms::SLABS;
            else if (!strcmp(argv[i+1], "marching"))
                ray_algorithm = RayAlgorithms::MARCHING;
            else if (!strcmp(argv[i+1], "bitmask"))
                ray_algorithm = RayAlgorithms::BITMASK;
            else {
                std::cout << "Bad algorithm name after the --algorithm,-a argument\n";
                exit(-1);
            }
            ++i;
        } else if (!std::strcmp(argv[i], "--step")) {
            // --step
            if (i+1 == argc) {
                std::cout << "Missing marching step after the --step argument\n";
                exit(-1);
            }
            try {
                marching_step = std::stod(argv[i+1]);
            } catch (std::invalid_argument const& e) {
                std::cout << "Bad argument provided to --step. Please provide a double.\n";
                exit(-1);
            }
            ++i;
        } else {
            std::cout << "Unknown argument provided: " << argv[i] << '\n';
            exit(-1);
        }
    }

    if (chunkPath == "") {
        std::cout << "Missing --chunk argument, can't proceed.\nSee the documentation to see what arguments you can provide this program.\n";
        exit(-1);
    }

    assert(std::filesystem::exists(chunkPath));
    assert(std::filesystem::exists(shapesPath));
    // assert(section >= -4 && section <= 15);
    // The Wiki is unclear about these values, maybe they are false
}

