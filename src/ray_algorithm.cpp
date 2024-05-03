/**
 * @file ray_algorithm.cpp
 */
#include "ray_algorithm.hpp"
#include "util.hpp"

/**
 * Helper function for slab algorithm
 * Checks if the ray (origin/direction) hits the box, and if so edits distance accordingly
 * Because an AABB has coordinates given in the frame of reference of the first vertex,
 * the origin too should be expressed locally in that frame
 */
bool slabsRayHitsBox(const Point& origin, const Point& direction, const AABB& box, double& distance) {
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
    if (!scene.inBounds(next_tile))
        return false;
    auto boxes = scene.getVoxel(next_tile).getContents();

    bool hits_something = false;
    double min_distance = HUGE_VAL;
    for (auto& box: boxes) {
        double distance_to_box;
        Point origin_relative = prev_point - Point(next_tile.x, next_tile.y, next_tile.z);
        if (slabsRayHitsBox(origin_relative, ray.getDirection(), box, distance_to_box)) {
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
            else if (ray.getDirection()[axis] > 0)
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

bool MarchingSlabAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    Point prev_point = ray.getLastTracePoint();

    // Unlike the classical algorithm, collision candidates may be in the current tile or in the next one
    auto current_tile = VoxelPosition(prev_point);
    auto boxes = scene.getVoxel(current_tile).getContents();

    bool hits_something = false;
    double min_distance = HUGE_VAL;

    for (auto& box: boxes) {
        double distance_to_box;
        Point origin_relative = prev_point - Point(current_tile.x, current_tile.y, current_tile.z);
        if (slabsRayHitsBox(origin_relative, ray.getDirection(), box, distance_to_box)) {
            hits_something = true;
            if (distance_to_box < min_distance)
                min_distance = distance_to_box;
        }
    }

    auto next_tile = VoxelPosition(prev_point + ray.getDirection()*this->step);
    if (!hits_something && current_tile != next_tile && scene.inBounds(next_tile)) {
        auto next_boxes = scene.getVoxel(next_tile).getContents();
        for (auto& box: next_boxes) {
            double distance_to_box;
            Point origin_relative = prev_point - Point(next_tile.x, next_tile.y, next_tile.z);
            if (slabsRayHitsBox(origin_relative, ray.getDirection(), box, distance_to_box)) {
                hits_something = true;
                if (distance_to_box < min_distance)
                    min_distance = distance_to_box;
            }
        }
    }

    if (hits_something) {
        // Advance to the AABB that is hit
        Point new_point(prev_point + ray.getDirection()*min_distance);
        ray.addTrace(new_point);
    } else {
        Point new_point(prev_point + ray.getDirection()*this->step);
        ray.addTrace(new_point);
    }

    return hits_something;
}

/**
 * TODO
 */
bool bitmaskRayHitsBox(const Point& new_pos, const Point& direction, const AABB& box, double& distance) {
    const Point radius = box.radius();

    // Equivalent of glm::sign
    Point sgn(
        direction.x() >= 0 ? -1 : 1,
        direction.y() >= 0 ? -1 : 1,
        direction.z() >= 0 ? -1 : 1
    );

    // Distance to plane
    Point d(
        radius.x() * sgn.x() - new_pos.x(),
        radius.y() * sgn.y() - new_pos.y(),
        radius.z() * sgn.z() - new_pos.z()
    );

    d.x() /= direction.x();
    d.y() /= direction.y();
    d.z() /= direction.z();

    bool test_x =
        (d.x() >= 0.)
            && (std::abs(new_pos.y() + direction.y() * d.x()) <= radius.y())
            && (std::abs(new_pos.z() + direction.z() * d.x()) <= radius.z());
    bool test_y =
        (d.y() >= 0.)
            && (std::abs(new_pos.z() + direction.z() * d.y()) <= radius.z())
            && (std::abs(new_pos.x() + direction.x() * d.y()) <= radius.x());
    bool test_z =
        (d.z() >= 0.)
            && (std::abs(new_pos.x() + direction.x() * d.z()) <= radius.x())
            && (std::abs(new_pos.y() + direction.y() * d.z()) <= radius.y());

    sgn = test_x ? Point(sgn.x(),0.,0.)
        : (test_y ? Point(0.,sgn.y(),0.)
            : Point(0.,0., test_z ? sgn.z() : 0));

    // Set the distance
    distance = (sgn.x() != 0) ? d.x() : ((sgn.y() != 0) ? d.y() : d.z());
    // normal = sgn;

    if (test_x || test_y || test_z)
        return true;
    else
        return false;
}

bool BitmaskAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    // Get the last trace point and get the associated voxel to test
    const Point ray_pos = ray.getLastTracePoint();
    VoxelPosition vp(ray_pos + (ray.getDirection()*1e-5));
    Voxel curr_voxel = scene.getVoxel(vp);

    // Only keep the closest hit
    bool hits_something = false;
    double min_distance = HUGE_VAL;

    // Check all the AABBs of the current voxel to check for intersection
    for (const AABB& box : curr_voxel.getContents()) {
        Point new_pos = ray_pos - (box.center() + Point(vp.x, vp.y, vp.z));
        double distance;
        if (bitmaskRayHitsBox(new_pos, ray.getDirection(), box, distance)) {
            if (distance < min_distance)
                min_distance = distance;
            hits_something = true;
        }
    }

    if (hits_something) {
        Point new_point(ray_pos + ray.getDirection()*min_distance);
        ray.addTrace(new_point);
        return true;
    } else {
        // No AABB hit, go to the next voxel
        double distance_to_next_voxel = HUGE_VAL;
        for (int axis=0; axis<3; ++axis) {
            // offset: how far along the current axis one should move to change tile
            double offset = fmod(ray_pos[axis], 1);
            if (offset == 0)
                offset = 1;
            else if (ray.getDirection()[axis] > 0)
                offset = 1 - offset;

            // distance: how far along the ray one should move to move by offset on the current axis
            double distance = offset / std::abs(ray.getDirection()[axis]);
            if (distance < distance_to_next_voxel)
                distance_to_next_voxel = distance;
        }
        Point new_point(ray_pos + ray.getDirection()*distance_to_next_voxel);
        ray.addTrace(new_point);
        return false;
    }
}

bool MarchingBitmaskAlgorithm::computeStep(Ray& ray, const SandboxScene& scene) {
    // Get the last trace point and get the associated voxel to test
    const Point ray_pos = ray.getLastTracePoint();
    VoxelPosition curr_tile(ray_pos);

    // Unlike the classical algorithm, collision candidates may be in the current tile or in the next one
    Voxel curr_voxel = scene.getVoxel(curr_tile);

    // Only keep the closest hit
    bool hits_something = false;
    double min_distance = HUGE_VAL;

    // Check all the AABBs of the current voxel to check for intersection
    for (const AABB& box : curr_voxel.getContents()) {
        Point new_pos = ray_pos - (box.center() + Point(curr_tile.x, curr_tile.y, curr_tile.z));
        double distance;
        if (bitmaskRayHitsBox(new_pos, ray.getDirection(), box, distance)) {
            if (distance < min_distance)
                min_distance = distance;
            hits_something = true;
        }
    }

    VoxelPosition next_tile(ray_pos + ray.getDirection()*this->step);
    if (!hits_something && curr_tile != next_tile && scene.inBounds(next_tile)) {
        Voxel next_boxes = scene.getVoxel(next_tile);
        for (const AABB& box : next_boxes.getContents()) {
            double distance;
            Point new_pos = ray_pos - (box.center() + Point(next_tile.x, next_tile.y, next_tile.z));
            if (bitmaskRayHitsBox(new_pos, ray.getDirection(), box, distance)) {
                if (distance < min_distance)
                    min_distance = distance;
                hits_something = true;
            }
        }
    }

    if (hits_something) {
        Point new_point(ray_pos + ray.getDirection()*min_distance);
        ray.addTrace(new_point);
        return true;
    } else {
        Point new_point(ray_pos + ray.getDirection()*this->step);
        ray.addTrace(new_point);
        return false;
    }
}

