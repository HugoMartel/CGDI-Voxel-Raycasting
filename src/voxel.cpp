/**
 * @file voxel.cpp
 */
#include "voxel.hpp"

std::ostream& operator<<(std::ostream& os, const AABB& box) {
    os << "AABB[" << box.minX << ", " << box.minY << ", " << box.minZ
       << "] -> [" << box.maxX << ", " << box.maxY << ", " << box.maxZ << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Voxel& v) {
    os << "Voxel[";
    for (const AABB& box : v.contents)
        os << box << ", ";
    os << ']';
    return os;
}
