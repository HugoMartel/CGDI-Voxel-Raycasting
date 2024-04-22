/**
 * @file main.cpp
 */
#include <iostream>

#include <memory>
#include <polyscope/polyscope.h>

#include "argparser.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "ray_algorithm.hpp"
#include "util.hpp"

// == GLOBALS
bool raystep_pressed = false;
bool rayreset_pressed = false;

std::unique_ptr<SandboxScene> scene;
std::unique_ptr<Ray> ray;
std::unique_ptr<RayAlgorithm> ray_algorithm;// defaults to SlabAlgorithm

// == FUNCTIONS
/**
 * Function called at the begining of the main code to initialize needed things.
 */
void init() {
    createSceneMeshes(*scene, "XYZ");
    createRayLine(*ray, "RAY");
}

/**
 * Draw function to call when changing fun.
 */
void draw() {
    updateRayLine(*ray, "RAY");
}

/**
 * Callback for polyscope's ImGui UI
 */
void uiCallback() {
    // Buttons definitions
    if (ImGui::Button("Ray step"))
        raystep_pressed = true;

    if (ImGui::Button("Reset ray"))
        rayreset_pressed = true;

    // Buttons callbacks
    if (raystep_pressed) {
        ray_algorithm->computeStep(*ray, *scene);
        draw();
        raystep_pressed = false;
    }
    if (rayreset_pressed) {
        ray->clearTrace();
        draw();
        rayreset_pressed = false;
    }
}

// == MAIN
int main(const int argc, const char** argv) {
    ArgParser args(argc, argv);

    // Load the JSON file into a scene
    if (args.verbose)
        std::cout << "[+] Parsing the chunk file into a scene\n";
    //SandboxScene scene(10,10,10);
    scene = std::make_unique<SandboxScene>(args.chunkPath, args.shapesPath, args.section);

    // Create a Ray
    ray = std::make_unique<Ray>(Point(-2.,4.5,4.5), Point(1.,0.,0.));

    // Create a Ray Shooting Algorithm
    ray_algorithm = std::make_unique<SlabAlgorithm>();

    // Initialize polyscope
    polyscope::init();

    // Build the Mesh
    if (args.verbose)
        std::cout << "[+] Creating the blocks into Polyscope's scene\n";

    // Finish setting up the initial Polyscope scene
    init();

    // Specify the callback for the top right UI
    polyscope::state::userCallback = uiCallback;

    // Give control to the polyscope gui
    if (args.verbose)
        std::cout << "[+] Starting the Polyscope GUI\n";
    polyscope::show();

    return EXIT_SUCCESS;
}
