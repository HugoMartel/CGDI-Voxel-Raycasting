/**
 * @file argparser.cpp
 */
#include "argparser.hpp"

#include <iostream>
#include <filesystem>
#include <cassert>
#include <cstring>

ArgParser::ArgParser(const int argc, const char** argv)
: chunkPath(""), shapesPath(BLOCK_SHAPES_FILE_PATH), section(0), verbose(false) {
    // Iterate on the arguments
    for (int i=1; i<argc; ++i) {
        if (!std::strcmp(argv[i], "--verbose")) {
            // --verbose
            verbose = true;
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
                std::cout << "Bad argument provided to --section,-s. Please provide an integer.";
                exit(-1);
            }
            ++i;
        } else {
            std::cout << "Unknown argument provided: " << argv[i] << '\n';
            exit(-1);
        }
    }

    assert(std::filesystem::exists(chunkPath));
    assert(std::filesystem::exists(shapesPath));
    // assert(section >= -4 && section <= 15);
    // The Wiki is unclear about these values, maybe they are false
}

