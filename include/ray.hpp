/**
 * @file ray.hpp
 */
#ifndef __RAYCAST_RAY__
#define __RAYCAST_RAY__

#include <string>

#include "geometry.hpp"
#include "voxel.hpp"

class Ray {
private:
    // Attributes
    /**
     * Point of origin of the ray.
     */
    Point origin;
    /**
     * Direction of the ray represented by a normalized Point.
     */
    Point direction;
    /**
     * Trace of Points occuring when shooting the ray.
     * @note Might not be a good idea for all ray shooting algorithms.
     */
    std::vector<Point> trace;

public:
    // Constructors
    /**
     * Basic constructor.
     * @param   ori     Point of origin of the ray.
     * @param   dir     Direction of the ray, should be a normalized point.
     */
    Ray(const Point& ori, const Point& dir) : origin(ori), direction(dir), trace(1, ori) {}

    // Methods
    /**
     * Adds a point to the trace.
     * @param   p   Point to add.
     */
    inline void addTrace(const Point& p) {
        trace.emplace_back(p);
    }
    /**
     * Getter for the trace.
     * @return  A copy of the trace.
     */
    inline std::vector<Point> getTrace() const {
        return trace;
    }
    /**
     * Getter for the trace.
     * @note    This version returns a reference!
     * @return  A reference to the trace.
     */
    inline std::vector<Point>& getTrace() {
        return trace;
    }
    /**
     * Getter for the last point added to the trace.
     * @return  A copy of the last point of the trace.
     */
    inline Point getLastTracePoint() const {
        return trace.back();
    }
    /**
     * Clears the trace completely and reinitializes it with the origin Point.
     */
    inline void clearTrace() {
        trace.clear();
        trace.emplace_back(origin);
    }

    /**
     * Getter for the origin point of the ray.
     * @return  A copy of the point.
     */
    inline Point getOrigin() const {
        return origin;
    }
    /**
     * Getter for the direction of the ray.
     * @return  A copy of the normalized direction point.
     */
    inline Point getDirection() const {
        return direction;
    }
    /**
     * Convert the ray's origin Point to a VoxelPosition in the scene.
     * @return  A VoxelPosition object.
     */
    inline VoxelPosition getVoxelPosition() const {
        return VoxelPosition((int)origin.x(), (int)origin.y(), (int)origin.z());
        // return VoxelPosition(origin + (direction*0.001));
    }
};

#endif//__RAYCAST_RAY__

