/**
 * @file ray_algorithm.cpp
 */
#include "ray_algorithm.hpp"

bool SlabAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    // TODO: write the actual algorithm
    Point prev_point = ray.getLastTracePoint();
    Point new_point(prev_point + ray.getDirection());
    std::cout << "NEW RAY TRACE POINT: " << new_point << '\n';//! DEBUG
    ray.addTrace(new_point);

    return false;
}

