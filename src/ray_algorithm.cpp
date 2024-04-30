/**
 * @file ray_algorithm.cpp
 */
#include "ray_algorithm.hpp"

/**
 * Helper function for slab algorithm
 * Checks if the ray (origin/direction) hits the box, and if so edits distance accordingly
 * Because an AABB has coordinates given in the frame of reference of the first vertex,
 * the origin too should be expressed locally in that frame
 */
bool rayHitsBox(const Point& origin, const Point& direction, const AABB& box, double& distance) {
    double t_near = -HUGE_VAL;
    double t_far = HUGE_VAL;

    // Repeat for every pair of parallel planes
    for(int axis=0; axis<3; ++axis) {
        // If the ray is parallel to the planes, check whether the origin lies between them
        if (direction[axis] == 0) {
            if (origin[axis] < box.min[axis] || origin[axis] > box.max[axis])
                return false;
            continue;
        }

        // Compute distance to both planes
        double t1 = (box.min[axis] - origin[axis]) / direction[axis];
        double t2 = (box.max[axis] - origin[axis]) / direction[axis];
        if (t1 > t2) {
            // Swap t1 and t2 if necessary
            t2 += t1;
            t1 = t2 - t1;
            t2 -= t1;
        }
        if (t1 > t_near)
            t_near = t1;
        if (t2 < t_far)
            t_far = t2;

        if (t_near > t_far)
            // Box is missed
            return false;
        if (t_far < 0)
            // Box is behind ray
            return false;
    }

    distance = t_near;
    return true;
}

bool SlabAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    Point prev_point = ray.getLastTracePoint();
    auto next_tile = VoxelPosition(prev_point + ray.getDirection()*1e-5);
    auto boxes = scene.getVoxel(next_tile).getContents();

    bool hits_something = false;
    double min_distance = HUGE_VAL;
    for (auto& box: boxes) {
        double distance_to_box;
        Point origin_relative = prev_point - Point(next_tile.x, next_tile.y, next_tile.z);
        if (rayHitsBox(origin_relative, ray.getDirection(), box, distance_to_box)) {
            hits_something = true;
            if (distance_to_box < min_distance)
                min_distance = distance_to_box;
        }
    }

    if (hits_something) {
        // Advance to the AABB that is hit
        Point new_point(prev_point + ray.getDirection()*min_distance);
        ray.addTrace(new_point);
    } else {
        // Advance to the next voxel
        double distance_to_next_voxel = HUGE_VAL;
        for (int axis=0; axis<3; ++axis) {
            // offset: how far along the current axis one should move to change tile
            double offset = fmod(prev_point[axis], 1);
            if (offset == 0)
                offset = 1;
            else if (ray.getDirection()[axis] < 0)
                offset = 1 - offset;

            // distance: how far along the ray one should move to move by offset on the current axis
            double distance = offset / std::abs(ray.getDirection()[axis]);
            if (distance < distance_to_next_voxel)
                distance_to_next_voxel = distance;
        }
        Point new_point(prev_point + ray.getDirection()*distance_to_next_voxel);
        ray.addTrace(new_point);
    }

    return hits_something;
}

bool BitmaskAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    const Point ray_pos = ray.getLastTracePoint();

    /* GLSL version provided in the paper
     * TODO: REMOVE THIS CODE
     */
    // this is only used to make sure that we are not right on an edge of a voxel
    Voxel curr_voxel = scene.getVoxel(VoxelPosition(ray_pos + (ray.getDirection()*1e5)));

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

