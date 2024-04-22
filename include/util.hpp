/**
 * @file util.hpp
 */
#ifndef __RAYCAST_UTIL__
#define __RAYCAST_UTIL__

#include <string>

#include "voxel.hpp"
#include "scene.hpp"
#include "ray.hpp"

/**
 * Converts a string containing a list of AABB into a std::vector or AABB.
 * @param   s   String to convert.
 * @return  std::vector<AABB>
 */
std::vector<AABB> str_to_aabbvector(const std::string& s);

/**
 * Constructs rectangular cuboids (with vertices and faces vectors) and adds them to the polyscope scene.
 * @param   scene   Sandbox scene object containing all the voxels' AABBs.
 * @param   prefix    Prefix name of the created polyscope object.
 */
void createSceneMeshes(const SandboxScene& scene, const std::string& prefix);

/**
 * Constructs a curve network to display a given ray in polyscope.
 * @param   ray     Ray object to add.
 * @param   name    Name of the polyscope object that will be created.
 */
void createRayLine(const Ray& ray, const std::string& name);

/**
 * Updates the curve network with the given name in the polyscope environment.
 * @param   ray     Ray object to add.
 * @param   name    Name of the polyscope object that will be created.
 */
void updateRayLine(const Ray& ray, const std::string& name);

#endif//__RAYCAST_UTIL__
