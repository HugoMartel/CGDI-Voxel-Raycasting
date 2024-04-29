/**
 * @file voxel.hpp
 */
#ifndef __RAYCAST_VOXEL__
#define __RAYCAST_VOXEL__

#include <iostream>
#include <vector>
#include <array>

#include "geometry.hpp"

/**
 * Structure to easily handle 3D coordinates of voxels.
 */
struct VoxelPosition {
public:
    // Attributes
    /**
     * Coordinates in the scene.
     */
    int x, y, z;

    // Constructors
    /**
     * Construct a VoxelPosition from 3 integers.
     * @param   x   Horizontal position.
     * @param   y   Vertical position.
     * @param   z   Depth position
     */
    VoxelPosition(const int x, const int y, const int z) : x(x), y(y), z(z) {}
    /**
     * Construct a VoxelPosition from an std::array of integers.
     * @param   xyz     Array storing the 3 coordinates in this order x, y and z.
     */
    VoxelPosition(const std::array<int, 3>& xyz) : x(xyz[0]), y(xyz[1]), z(xyz[2]) {}
    /**
     * Construct a VoxelPosition from a Point casting float coordinates to int.
     * @param   xyz     Array storing the 3 coordinates in this order x, y and z.
     */
    VoxelPosition(const Point& p) : x((int)p.x()), y((int)p.y()), z((int)p.z()) {}

    // Operators
    /**
     * Equality operator between two positions.
     * @param   pos     Other VoxelPosition to compare to.
     * @return  True if equal, false otherwise.
     */
    inline bool operator==(const VoxelPosition& pos) const {
        return (x == pos.x && y == pos.y && z == pos.z);
    }
};

/**
 * Structure storing an Axis Aligned Bounding Box.
 */
struct AABB {
public:
    // Attributes
    /**
     * Minimum point in [0,1]x[0,1]x[0,1].
     */
    // double minX, minY, minZ;
    Point min;
    /**
     * Maximum point in [0,1]x[0,1]x[0,1].
     */
    // double maxX, maxY, maxZ;
    Point max;

    // Constructors
    /**
     * Constructs an AABB struct from its 6 bounding parameters.
     * @param   minX   Horizontal minimum.
     * @param   minY   Vertical minimum.
     * @param   minZ   Depth minimum.
     * @param   maxX   Horizontal maximum.
     * @param   maxY   Vertical maximum.
     * @param   maxZ   Depth maximum.
     */
    AABB(
        const double& minX, const double& minY, const double& minZ,
        const double& maxX, const double& maxY, const double& maxZ
    ) : min(minX, minY, minZ), max(maxX, maxY, maxZ) {}
    /**
     * Constructs an AABB struct given 2 points.
     * @param   min     Minimum point.
     * @param   max     Maximum point.
     */
    AABB(const Point& min, const Point& max) : min(min), max(max) {}
    /**
     * Constructs an AABB struct from its center and a single side length (CUBE).
     * @param   centerX     Horizontal center.
     * @param   centerY     Vertical center.
     * @param   centerZ     Depth center.
     * @param   sideLenghth Length of every side of the bounding cube.
     */
    AABB(
        const double& centerX, const double& centerY, const double& centerZ,
        const double& sideLength
    ) : min(centerX-sideLength/2., centerY-sideLength/2., centerZ-sideLength/2.),
        max(centerX+sideLength/2., centerY+sideLength/2., centerZ+sideLength/2.) {}

    // Methods
    /**
     * Get the radius in each direction of the AABB.
     * TODO
     */
    Point radius() const{
        return Point((max.x()-min.x())/2., (max.y()-min.y())/2., (max.z()-min.z())/2.);
    }
    /**
     * Get the center point of the AABB.
     * TODO
     */
    Point center() const{
        return Point((max.x()+min.x())/2., (max.y()+min.y())/2., (max.z()+min.z())/2.);
    }
};

/**
 * Stream writing operator override for a bouding box object.
 * @param   os      Stream object to write into.
 * @param   box     AABB to display.
 * @return  Stream where the box has been written.
 */
std::ostream& operator<<(std::ostream& os, const AABB& box);

/**
 * Voxel class storing geometry elements using AABB.
 */
class Voxel {
private:
    // Attributes
    /**
     * Bounding boxes of the voxel.
     */
    std::vector<AABB> contents;

public:
    // Constructors
    /**
     * Default constructor building an empty Voxel.
     */
    Voxel() : contents(std::vector<AABB>()) {}
    /**
     * Simple constructor for Voxels only having one AABB.
     * @param   box     Bounding box to push in the Voxel's vector.
     */
    Voxel(const AABB& box) : contents(std::vector<AABB>(1, box)) {}
    /**
     * Constructor taking a vector of AABBs and copying them into a new Voxel.
     * @param   contents    Contents to copy to this voxel's contents.
     */
    Voxel(const std::vector<AABB>& contents) : contents(contents) {}

    // Methods
    /**
     * Returns a reference to the contents of the Voxel.
     * @return  Reference to a std::vector.
     */
    inline std::vector<AABB>& getContents() {
        return contents;
    }
    /**
     * Returns an iterator to the begining of the contents vector.
     * @return  std::vector::begin().
     */
    inline std::vector<AABB>::iterator begin() {
        return contents.begin();
    }
    /**
     * Returns an iterator to the end of the contents vector.
     * @return  std::vector::end().
     */
    inline std::vector<AABB>::iterator end() {
        return contents.end();
    }
    /**
     * Adds a new bounding box to the contents of the voxel.
     * @param   box     Bounding Box to add to the contents.
     */
    inline void addAABB(const AABB& box) {
        contents.emplace_back(box);
    }
    /**
     * Tests if the contents of the voxel are empty (no AABB).
     * @return true or false.
     */
    inline bool isEmpty() const {
        return contents.empty();
    }

    // Friend functions
    friend std::ostream& operator<<(std::ostream& os, const Voxel& v);
};

/**
 * Stream writing operator override for a voxel object.
 * @param   os      Stream object to write into.
 * @param   v       Voxel to display.
 * @return  Stream where the box has been written.
 */
std::ostream& operator<<(std::ostream& os, const Voxel& v);

#endif//__RAYCAST_VOXEL__

