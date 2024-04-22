/**
 * @file util.cpp
 */
#include "util.hpp"

#include <polyscope/surface_mesh.h>
#include <polyscope/curve_network.h>


std::vector<AABB> str_to_aabbvector(const std::string& s) {
    unsigned long curr=1;// Skip the first [
    unsigned long start;
    double x_min, y_min, z_min, x_max, y_max, z_max;
    std::vector<AABB> output;

    while (curr<s.length()-1) {
        curr += 5;// Skip AABB[

        // Get the x min coord
        start = curr;
        while (s[curr++] != ',');
        x_min = stod(s.substr(start, curr-start-1));

        ++curr;// Skip ' '

        // Get the y min coord
        start = curr;
        while (s[curr++] != ',');
        y_min = stod(s.substr(start, curr-start-1));

        ++curr;// Skip ' '

        // Get the z min coord
        start = curr;
        while (s[curr++] != ']');
        z_min = stod(s.substr(start, curr-start-1));

        curr += 5;// Skip " -> ["

        // Get the x max coord
        start = curr;
        while (s[curr++] != ',');
        x_max = stod(s.substr(start, curr-start-1));

        ++curr;// Skip ' '

        // Get the y max coord
        start = curr;
        while (s[curr++] != ',');
        y_max = stod(s.substr(start, curr-start-1));

        ++curr;// Skip ' '

        // Get the z max coord
        start = curr;
        while (s[curr++] != ']');
        z_max = stod(s.substr(start, curr-start-1));

        output.emplace_back(x_min, y_min, z_min, x_max, y_max, z_max);

        curr += 2;// Skip ", "
    }

    return output;
}

void createSceneMeshes(const SandboxScene& scene, const std::string& name) {
    // Cube faces to make a correctly oriented cube given arrays of vertices
    std::vector<Face> box_faces({
        {0,3,2,1},// Front
        {0,1,5,4},// Bottom
        {4,5,6,7},// Back
        {2,3,7,6},// Top
        {0,4,7,3},// Right
        {1,2,6,5} // Left
    });

    unsigned int cube_counter = 0;

    // Test scene
    for (int x=0; x<CHUNK_SIDE_SIZE; ++x) {
        for (int y=0; y<CHUNK_SIDE_SIZE; ++y) {
            for (int z=0; z<CHUNK_SIDE_SIZE; ++z) {
                for (AABB box : scene.getVoxel(VoxelPosition(x,y,z))) {
                    /* 1x1x1 cube: {
                     {0.,0.,0.},
                     {1.,0.,0.},
                     {1.,1.,0.},
                     {0.,1.,0.},
                     {0.,0.,1.},
                     {1.,0.,1.},
                     {1.,1.,1.},
                     {0.,1.,1.}
                    }
                    We simply map the AABB's min on 0s and max on 1s
                    */
                    std::vector<Vertex> box_vertices({
                        {box.minX + x, box.minY + y, box.minZ + z},
                        {box.maxX + x, box.minY + y, box.minZ + z},
                        {box.maxX + x, box.maxY + y, box.minZ + z},
                        {box.minX + x, box.maxY + y, box.minZ + z},
                        {box.minX + x, box.minY + y, box.maxZ + z},
                        {box.maxX + x, box.minY + y, box.maxZ + z},
                        {box.maxX + x, box.maxY + y, box.maxZ + z},
                        {box.minX + x, box.maxY + y, box.maxZ + z},
                    });
                    polyscope::registerSurfaceMesh(
                        name+' '+std::to_string(x)+' '+std::to_string(y)+' '+std::to_string(z)
                            +", ID: "+std::to_string(cube_counter++),
                        box_vertices,
                        box_faces);
                }
            }
        }
    }
}

void createRayLine(const Ray& ray, const std::string& name) {
    Point origin = ray.getOrigin();
    Point far_point = (ray.getOrigin()+ray.getDirection()*(CHUNK_SIDE_SIZE+2));
    polyscope::registerCurveNetworkLine(
        name,
        std::array<Point,2>{origin, far_point}
    );
    // polyscope::registerCurveNetworkLine(name, ray.trace);
}

void updateRayLine(const Ray& ray, const std::string& name) {
    // Overwrite the previous curve network
    polyscope::registerCurveNetworkLine(
        name,
        ray.getTrace()
    );
}
