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

bool BitmaskAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    const Point ray_pos = ray.getLastTracePoint();

    // Check if the ray is outside of the scene
    if (ray_pos.x() < 0. || ray_pos.y() < 0. || ray_pos.z() < 0. || ray_pos.x() >= CHUNK_SIDE_SIZE
       || ray_pos.y() >= CHUNK_SIDE_SIZE || ray_pos.z() >= CHUNK_SIDE_SIZE) {
        // if so, simply go to the next "voxel".
        Point new_point(ray_pos + ray.getDirection());
        std::cout << "NEW RAY TRACE POINT: " << new_point << '\n';//! DEBUG
        ray.addTrace(new_point);

        return false;
    }

    /* GLSL version provided in the paper
     * TODO: REMOVE THIS CODE
     */
    // TODO: check if the addition of a fraction of the ray direction can be a problem
    // this is only used to make sure that we are not right on an edge of a voxel
    Voxel curr_voxel = scene.getVoxel(VoxelPosition(ray_pos + (ray.getDirection()*0.001)));

    for (const AABB& box : curr_voxel.getContents()) {
        std::cout << "CHECKING: " << box << '\n';//! DEBUG
        const Point radius = box.radius();

        // ray.origin = ray.origin - box.center;
        // Point new_pos = ray_pos - box.center();
        Point new_pos = Point(ray_pos.x() - (int)ray_pos.x(),
                              ray_pos.y() - (int)ray_pos.y(),
                              ray_pos.z() - (int)ray_pos.z()) - radius;
        std::cout << "NEW_POS: " << new_pos << '\n';

        // if (oriented) {
        //     ray.dir *= box.rot;
        //     ray.origin *= box.rot;
        // }

        // float winding =
        //     canStartInBox && (max(abs(ray.origin) * box.invRadius) < 1.0)
        //     ? -1 : 1;

        // vec3 sgn = -sign(ray.dir);
        Point sgn;
        sgn.x() = ray.getDirection().x() >= 0 ? -1 : 1;
        sgn.y() = ray.getDirection().y() >= 0 ? -1 : 1;
        sgn.z() = ray.getDirection().z() >= 0 ? -1 : 1;

        // Distance to plane
        // vec3 d = box.radius * winding * sgn - ray.origin;
        Point d = - new_pos;
        d.x() += radius.x() * sgn.x();
        d.y() += radius.y() * sgn.y();
        d.z() += radius.z() * sgn.z();

        std::cout << "d: " << d << '\n';//! DEBUG

        // if (oriented)
        //     d /= ray.dir;
        // else
        //     d *= _invRayDir;
        d.x() /= ray.getDirection().x();
        d.y() /= ray.getDirection().y();
        d.z() /= ray.getDirection().z();

        std::cout << "d: " << d << '\n';//! DEBUG

    // # define TEST(U, VW) (d.U >= 0.0) && \
                            all(lessThan(abs(ray.origin.VW + ray.dir.VW * d.U), box.radius.VW))

    //     bvec3 test = bvec3(TEST(x, yz), TEST(y, zx), TEST(z, xy));
        bool test_x =
            (d.x() >= 0.)
            && (std::abs(new_pos.y() + ray.getDirection().y() * d.x()) <= radius.y())
            && (std::abs(new_pos.z() + ray.getDirection().z() * d.x()) <= radius.z());
        bool test_y =
            (d.y() >= 0.)
                && (std::abs(new_pos.z() + ray.getDirection().z() * d.y()) <= radius.z())
                && (std::abs(new_pos.x() + ray.getDirection().x() * d.y()) <= radius.x());
        bool test_z =
            (d.z() >= 0.)
                && (std::abs(new_pos.x() + ray.getDirection().x() * d.z()) <= radius.x())
                && (std::abs(new_pos.y() + ray.getDirection().y() * d.z()) <= radius.y());
    //     sgn = test.x
    //         ? vec3(sgn.x,0,0) : (test.y ? vec3(0,sgn.y,0) : vec3(0,0,test.z ? sgn.z:0));
        sgn = test_x ? Point(sgn.x(),0.,0.)
            : (test_y ? Point(0.,sgn.y(),0.)
                : Point(0.,0., test_z ? sgn.z() : 0));
        std::cout << "SGN: " << sgn << '\n';//! DEBUG

    // # undef TEST

        // distance = (sgn.x != 0) ? d.x : ((sgn.y != 0) ? d.y : d.z);
        // normal = oriented ? (box.rot * sgn) : sgn;

        // if ((sgn.x() != 0.) || (sgn.y() != 0.) || (sgn.z() != 0.))
        if (test_x || test_y || test_z) {
            ray.addTrace(ray_pos);
            return true;
        }
    }

    // No AABB hit, simply go to the next voxel
    Point new_point(ray_pos + ray.getDirection());
    std::cout << "NEW RAY TRACE POINT: " << new_point << '\n';//! DEBUG
    ray.addTrace(new_point);
    return false;
}

