/**
 * @file main.cpp
 */
#include <iostream>

#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>

#include "argparser.hpp"
#include "scene.hpp"

// == DEFINES

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
int main(const int argc, const char** argv) {
    ArgParser args(argc, argv);

    // Load the JSON file into a scene
    if (args.verbose)
        std::cout << "[+] Parsing the chunk file into a scene\n";
    //SandboxScene scene(10,10,10);
    SandboxScene scene(args.chunkPath, args.blocksPath, args.section);

    // Initialize polyscope
    polyscope::init();

    // Build the Mesh
    if (args.verbose)
        std::cout << "[+] Creating the blocks into Polyscope's scene\n";
    scene.createBlocks("XYZ");

    // Finish setting up the initial Polyscope scene 
    init();
    draw();

    // Specify the callback for the top right UI
    polyscope::state::userCallback = uiCallback;

    // Give control to the polyscope gui
    if (args.verbose)
        std::cout << "[+] Starting the Polyscope GUI\n";
    polyscope::show();

    return EXIT_SUCCESS;
}
