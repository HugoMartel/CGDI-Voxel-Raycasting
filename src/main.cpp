/**
 * @file main.cpp
 */
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>

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
        const Point ray_pos = ray->getLastTracePoint();
        if (
            ray_pos.x() >= 0. && ray_pos.y() >= 0. && ray_pos.z() >= 0.
            && ray_pos.x() < CHUNK_SIDE_SIZE && ray_pos.y() < CHUNK_SIDE_SIZE
            && ray_pos.z() < CHUNK_SIDE_SIZE
        ) {
            ray_algorithm->computeStep(*ray, *scene);
            draw();
        }
        raystep_pressed = false;
    }
    if (rayreset_pressed) {
        ray->reset();
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
    ray = std::make_unique<Ray>(Point(8.,5.5,4.5), Point(1.,0.,0.));

    // Create a Ray Shooting Algorithm
    switch (args.ray_algorithm) {
        case RayAlgorithms::SLABS:
        ray_algorithm = std::make_unique<SlabAlgorithm>();
        break;
    case RayAlgorithms::BITMASK:
        ray_algorithm = std::make_unique<BitmaskAlgorithm>();
        break;
    }

    // Initialize polyscope
    polyscope::init();
    polyscope::options::automaticallyComputeSceneExtents = false;
    polyscope::state::lengthScale = 5.;
    polyscope::state::boundingBox = std::tuple<glm::vec3, glm::vec3>{
        {0., 0., 0.}, {CHUNK_SIDE_SIZE, CHUNK_SIDE_SIZE, CHUNK_SIDE_SIZE}
    };

    // Build the Mesh
    if (args.verbose)
        std::cout << "[+] Creating the blocks into Polyscope's scene\n";

    // Finish setting up the initial Polyscope scene
    init();

    if (args.benchmark) {
        // Shoot N rays and measure the execution time.
        constexpr int N = 100000;
        const std::string output_filename = 
            "benchmark_"+std::to_string(N)+'_'+convert_to_string(args.ray_algorithm)+".txt";
        std::ofstream output(output_filename, std::ios_base::out);

        if (args.verbose)
            std::cout << "[+] Starting the Benchmark\n";
        for (int i=0; i<N; ++i) {
            // Shoot a ray until it intersects or goes out of the scene
            ray->reset();
            output << ray->getOrigin() << ';' << ray->getDirection() << '|';
            Point ray_pos = ray->getLastTracePoint();
            while (
                ray_pos.x() >= 0. && ray_pos.y() >= 0. && ray_pos.z() >= 0.
                && ray_pos.x() < CHUNK_SIDE_SIZE && ray_pos.y() < CHUNK_SIDE_SIZE
                && ray_pos.z() < CHUNK_SIDE_SIZE
            ) {
                // Actual benchmark of the algorithm step
                const auto t_start = std::chrono::high_resolution_clock::now();
                const bool found_inter = ray_algorithm->computeStep(*ray, *scene);
                const auto t_end = std::chrono::high_resolution_clock::now();
                // Write results to file
                output << ray_pos << ';';
                output << std::chrono::duration<double, std::micro>(t_end - t_start) << ';';
                ray_pos = ray->getLastTracePoint();

                if (found_inter)
                    break;
            }
            output << '\n';
        }
        if (args.verbose)
            std::cout << "[+] Benchmark written to " << output_filename << '\n';
    } else {
        // Specify the callback for the top right UI
        polyscope::state::userCallback = uiCallback;

        // Give control to the polyscope gui
        if (args.verbose)
            std::cout << "[+] Starting the Polyscope GUI\n";
        polyscope::show();
    }

    return EXIT_SUCCESS;
}
