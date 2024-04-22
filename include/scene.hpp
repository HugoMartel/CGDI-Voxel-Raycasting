/**
 * @file scene.hpp
 */
#ifndef __RAYCAST_SCENE__
#define __RAYCAST_SCENE__

#include <string>

#include "voxel.hpp"

#define CHUNK_SIDE_SIZE 16

/**
 * 3D Scene storage class to use.
 */
template<typename T>
using Lattice3D = std::vector<std::vector<std::vector<T>>>;

/**
 * Class containing the entire information of the loaded scene.
 */
class SandboxScene {
private:
    // Attributes
    /**
     * 3D voxel scene containing Voxel objects identified by their coordinates.
     */
    Lattice3D<Voxel> voxels;

public:
    // Constructors
    /**
     * Sandbox scene constructor.
     * @param   width   Width of the scene.
     * @param   height  Height of the scene.
     * @param   depth   Depth of the scene.
     */
    SandboxScene(const int width, const int height, const int depth) {
        voxels = Lattice3D<Voxel>(width, std::vector<std::vector<Voxel>>(
            height, std::vector<Voxel>(depth, Voxel()))
        );
    }
    /**
     * Sandbox scene constructor that takes a chunk JSON file as input and contructs a scene from it.
     * @param   chunkPath       File location of the chunk JSON file.
     * @param   blocksPath      File location of all the blocks AABB and properties.
     * @param   chosen_section  Section number to load.
     */
    SandboxScene(const std::string& chunkPath, const std::string& blocksPath,
                 const int chosen_section);

    // Methods
    /**
     * Getter for a voxel in the scene given a position.
     * @param   position    Position of the requested voxel.
     * @return  Voxel object found at that given position.
     */
    inline Voxel getVoxel(const VoxelPosition& position) const {
        return voxels[position.y][position.z][position.x];
    }
    /**
     * Getter for a voxel in the scene given a position.
     * @note    This version returns a reference!
     * @param   position    Position of the requested voxel.
     * @return  Reference to the Voxel object found at that given position.
     */
    inline Voxel& getVoxel(const VoxelPosition& position) {
        return voxels[position.y][position.z][position.x];
    }
    /**
     * Setter of a voxel at a given position in the scene.
     * @param   position    Position of the voxel to set.
     * @param   voxel       Voxel to set.
     */
    void setVoxel(const VoxelPosition& position, const Voxel& voxel) {
        voxels[position.y][position.z][position.x] = voxel;
    }
};

#endif//__RAYCAST_SCENE__

