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

    // Build the Mesh
    //! Example TEST cube
    std::vector<std::array<double,3>> test_cube_vertices({
        {0.,0.,0.},
        {1.,0.,0.},
        {1.,1.,0.},
        {0.,1.,0.},
        {0.,0.,1.},
        {1.,0.,1.},
        {1.,1.,1.},
        {0.,1.,1.}
    });
    std::vector<std::vector<size_t>> test_cube_faces({
        {0,3,2,1},// Front
        {0,1,5,4},// Bottom
        {4,5,6,7},// Back
        {2,3,7,6},// Top
        {0,4,7,3},// Right
        {1,2,6,5} // Left
    });

    // Initialize polyscope
    polyscope::init();

    // Register the mesh with polyscope
    polyscope::SurfaceMesh* poly_surface = polyscope::registerSurfaceMesh(
        "Test cube",
        test_cube_vertices,
        test_cube_faces
    );

    // Finish setting up the initial Polyscope scene 
    init();
    draw();

    // Specify the callback for the top right UI
    polyscope::state::userCallback = uiCallback;

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}
