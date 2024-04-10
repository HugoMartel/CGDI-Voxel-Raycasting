/**
 * @file util.hpp
 */
#ifndef __RAYCAST_UTIL__
#define __RAYCAST_UTIL__

#include <string>

#include "voxel.hpp"

/**
 * Converts a string containing a list of AABB into a std::vector or AABB.
 * @param   s   String to convert.
 * @return  std::vector<AABB>
 */
std::vector<AABB> str_to_aabbvector(const std::string& s);


#endif//__RAYCAST_UTIL__
