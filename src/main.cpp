/**
 * @file main.cpp
 */
#include <iostream>
#include <filesystem>
#include <cassert>

#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>

#include "scene.hpp"

// == DEFINES
#define BLOCKS_FILE_LOCATION "../voxels/1_20_1_blocks.json"

// == FUNCTIONS
/**
 * Function called at the begining of the main code to initialize needed things.
 */
void init() {
    // TODO
}

/**
 * Draw function to call when changing fun.
 */
void draw() {
    // TODO
}

/**
 * Callback for polyscope's ImGui UI
 */
void uiCallback() {
    //TODO
}

// == MAIN
int main(int argc, char* argv[]) {
    assert(std::filesystem::exists(BLOCKS_FILE_LOCATION));

    // Load chunk file
    if (argc == 1) {
        std::cout << "Missing JSON chunk file to load\n";
        exit(0);
    } else if (!std::filesystem::exists(argv[1])) {
        std::cerr << "No file located at " << argv[1] << '\n';
        exit(-1);
    }

    // Load the JSON file into a scene
    //SandboxScene scene(10,10,10);
    SandboxScene scene(argv[1], BLOCKS_FILE_LOCATION);

    // Initialize polyscope
    polyscope::init();

    // Build the Mesh
    polyscope::SurfaceMesh* chunkSurface = scene.createMesh("Chunk");

    // Finish setting up the initial Polyscope scene 
    init();
    draw();

    // Specify the callback for the top right UI
    polyscope::state::userCallback = uiCallback;

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}
