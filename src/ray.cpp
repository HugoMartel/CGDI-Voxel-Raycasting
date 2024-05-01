/**
 * @file ray.cpp
 */
#include "ray.hpp"

#include <random>

#include "scene.hpp"

void Ray::reset() {
    std::random_device rd;
    std::default_random_engine gen(rd()); 
    std::uniform_real_distribution<double> ori_distrib(0, CHUNK_SIDE_SIZE-1);
    std::uniform_real_distribution<double> dir_distrib(-1, 1);

    origin.x() = ori_distrib(gen);
    origin.y() = ori_distrib(gen);
    origin.z() = ori_distrib(gen);
    direction.x() = dir_distrib(gen);
    direction.y() = dir_distrib(gen);
    direction.z() = dir_distrib(gen);
    direction /= direction.norm1();

    clearTrace();
}

