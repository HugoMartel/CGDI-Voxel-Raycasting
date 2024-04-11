/**
 * @file geometry.hpp
 */
#ifndef __RAYCAST_GEOMETRY__
#define __RAYCAST_GEOMETRY__

#include <array>
#include <vector>
#include <cmath>

using Vertex = std::array<double,3>;
using Face = std::vector<size_t>;
using Matrix4d = std::array<std::array<double,4>,4>;


/**
 * TODO
 */
class Point {
private:
    // Attributes
    std::array<double,3> xyz;

public:
    // Constructors
    /**
     * TODO
     */
    Point() : xyz({0., 0., 0.}) {}
    /**
     * TODO
     */
    Point(const double& x, const double& y, const double& z) : xyz({x, y, z}) {}
    /**
     *
     */
    Point(const std::array<double,3>& arr) : xyz(arr) {}
    /**
     * TODO
     */
    Point(const Point& a, const Point& b)
        : xyz({b.xyz[0]-a.xyz[0], b.xyz[1]-a.xyz[1], b.xyz[2]-a.xyz[2]}) {}

    // Operators
    /**
     * TODO
     */
    inline Point operator+(const Point& p) const {
        return Point(xyz[0]+p.xyz[0], xyz[1]+p.xyz[1], xyz[2]+p.xyz[2]);
    }
    /**
     * TODO
     */
    inline Point operator-(const Point& p) const {
        return Point(xyz[0]-p.xyz[0], xyz[1]-p.xyz[1], xyz[2]-p.xyz[2]);
    }
    /**
     * TODO
     */
    inline Point operator*(const double& d) const {
        return Point(xyz[0]*d, xyz[1]*d, xyz[2]*d);
    }
    /**
     * TODO
     */
    inline Point operator/(const double& d) {
        return Point(xyz[0]/d, xyz[1]/d, xyz[2]/d);
    }
    /**
     * TODO
     */
    inline Point& operator+=(const Point& p) {
        xyz[0] += p.xyz[0], xyz[1] += p.xyz[1], xyz[2] += p.xyz[2]; 
        return *this;
    }
    inline Point& operator-=(const Point& p) {
        xyz[0] -= p.xyz[0], xyz[1] -= p.xyz[1], xyz[2] -= p.xyz[2]; 
        return *this;
    }
    inline Point& operator*=(const double& d) {
        xyz[0] *= d, xyz[1] *= d, xyz[2] *= d;
        return *this;
    }
    inline Point& operator/=(const double& d) {
        xyz[0] /= d, xyz[1] /= d, xyz[2] /= d; 
        return *this;
    }

    // Methods
    /**
     * TODO
     */
    inline double x() const {
        return xyz[0];
    }
    /**
     * TODO
     */
    inline double& x() {
        return xyz[0];
    }
    /**
     * TODO
     */
    inline double y() const {
        return xyz[1];
    }
    /**
     * TODO
     */
    inline double& y() {
        return xyz[1];
    }
    /**
     * TODO
     */
    inline double z() const {
        return xyz[2];
    }
    /**
     * TODO
     */
    inline double& z() {
        return xyz[2];
    }
    /**
     * TODO
     */
    inline double operator[](const int i) const {
        return xyz[i];
    }
    /**
     * TODO
     */
    inline double& operator[](const int i) {
        return xyz[i];
    }
    /**
     * TODO
     * @note maybe return reference?
     */
    inline std::array<double,3> to_vertex() const {
        return xyz;
    }

    /**
     * TODO
     */
    inline double dot(const Point& p) const {
        return xyz[0]*p.xyz[0] + xyz[1]*p.xyz[1] + xyz[2]*p.xyz[2];
    }
    /**
     * TODO
     */
    inline Point cross(const Point& p) const {
        return Point(
            xyz[1]*p.xyz[2] - p.xyz[1]*xyz[2],
            xyz[2]*p.xyz[0] - p.xyz[2]*xyz[0],
            xyz[0]*p.xyz[1] - p.xyz[0]*xyz[1]
        );
    }
    /**
     * TODO
     */
    inline double norm1() const {
        return std::abs(xyz[0]) + std::abs(xyz[1]) + std::abs(xyz[2]);
    }
    /**
     * TODO
     */
    inline double norm2() const {
        return std::sqrt(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    }
    /**
     * TODO
     */
    inline double normInf() const {
        return std::max(std::abs(xyz[0]), std::max(std::abs(xyz[1]), std::abs(xyz[2])));
    }
    /**
     *
     */
    inline Point& transform(const Matrix4d& m) {
        double new_x = xyz[0]*m[0][0] + xyz[1]*m[0][1] + xyz[2]*m[0][2] + m[0][3];
        double new_y = xyz[0]*m[1][0] + xyz[1]*m[1][1] + xyz[2]*m[1][2] + m[1][3];
        double new_z = xyz[0]*m[2][0] + xyz[1]*m[2][1] + xyz[2]*m[2][2] + m[2][3];
        xyz[0] = new_x, xyz[1] = new_y, xyz[2] = new_z; 
        return *this;
    }
};

/**
 *
 */
Vertex vertex_transform(const Vertex& v, const Matrix4d& m) {
    
    return {
        v[0]*m[0][0] + v[1]*m[0][1] + v[2]*m[0][2] + m[0][3],
        v[0]*m[1][0] + v[1]*m[1][1] + v[2]*m[1][2] + m[1][3],
        v[0]*m[2][0] + v[1]*m[2][1] + v[2]*m[2][2] + m[2][3]
    };
}

#endif//__RAYCAST_UTIL__

