/**
 * @file util.cpp
 */
#include "util.hpp"

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

