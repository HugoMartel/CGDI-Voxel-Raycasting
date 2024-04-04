#include <iostream>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

#include <geometrycentral/utilities/vector3.h>
#include <geometrycentral/surface/meshio.h>
#include <geometrycentral/surface/manifold_surface_mesh.h>
#include <geometrycentral/surface/vertex_position_geometry.h>
#include <geometrycentral/surface/halfedge_element_types.h>
#include <geometrycentral/numerical/linear_algebra_types.h>

#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>

#include "scene.cpp"

// == DEFINES

// == GLOBAL
std::unique_ptr<geometrycentral::surface::ManifoldSurfaceMesh> mesh;
std::unique_ptr<geometrycentral::surface::VertexPositionGeometry> geometry;
polyscope::SurfaceMesh *poly_surface;

// == FUNCTIONS
/**
 * Function called at the begining of the main code to initialize global objects.
 */
void initObjects() {
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
int main(int argc, char **argv) {
    // Initialize polyscope
    polyscope::init();

    // Load mesh
    if (argc == 1) {
        std::cout << "Missing OBJ file to load\n";
        exit(0);
    }
    std::tie(mesh, geometry) = geometrycentral::surface::readManifoldSurfaceMesh(argv[1]);

    // Register the mesh with polyscope
    poly_surface = polyscope::registerSurfaceMesh(
        "Input obj",
        geometry->inputVertexPositions,
        mesh->getFaceVertexList(),
        polyscopePermutations(*mesh)
    );

    initObjects();
    draw();

    // Specify the callback
    polyscope::state::userCallback = uiCallback;

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}
