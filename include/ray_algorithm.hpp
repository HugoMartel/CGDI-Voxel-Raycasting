/**
 * @file ray_algorithm.hpp
 */
#ifndef __RAYCAST_RAY_ALGORITHM__
#define __RAYCAST_RAY_ALGORITHM__

#include "ray.hpp"
#include "scene.hpp"
#include "geometry.hpp"


/**
 * Mother class for the ray algorithms used to compute a step for a given ray
 */
class RayAlgorithm {
public:
    /**
     * Call function used when we want to do a step of the algorithm
     * @note VIRTUAL PURE FUNCTIONS ARE BAD FOR PERFORMANCE IN C++, try to find a substitute later
     * @param   ray     Ray to continue
     * @param   scene   Voxel scene to use to check for intersections
     * @return  True if an intersection was found
     */
    virtual bool computeStep(Ray& ray, const SandboxScene& scene) = 0;
};


/**
 * Classical implementation of the slab algorithm for a ray shooting AABB intersection problem.
 */
class SlabAlgorithm : public RayAlgorithm {
public:
    /**
     * TODO explain the algorithm
     * @param   ray     Ray to continue
     * @param   scene   Voxel scene to use to check for intersections
     * @return  True if an intersection was found
     */
    bool computeStep(Ray& ray, const SandboxScene& scene);
};


#endif//__RAYCAST_RAY_ALGORITHM__

