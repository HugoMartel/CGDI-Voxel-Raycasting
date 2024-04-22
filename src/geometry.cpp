/**
 * @file geometry.cpp
 */
#include "geometry.hpp"

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "Point{" << p.xyz[0] << ',' << p.xyz[1] << ',' << p.xyz[2] << '}';
    return os;
}

