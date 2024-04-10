/**
 * @file scene.hpp
 */
#ifndef __RAYCAST_SCENE__
#define __RAYCAST_SCENE__

#include <string>

#include "voxel.hpp"

template<typename T>
using Lattice3D = std::vector<std::vector<std::vector<T>>>;

/**
 * Class containing the entire information of the loaded scene.
 */
class SandboxScene {
private:
    // Attributes
    /**
     * 3D voxel scene containing Voxel objects identified by their coordinate.
     */
    Lattice3D<Voxel> voxels;

public:
    // Constructors
    /**
     * Sandbox scene constructor.
     * @param   width   Width of the voxels scene.
     * @param   height  Height of the voxels scene.
     * @param   depth   Depth of the voxels scene.
     */
    SandboxScene(const int width, const int height, const int depth) {
        voxels = Lattice3D<Voxel>(width, std::vector<std::vector<Voxel>>(
            height, std::vector<Voxel>(depth, Voxel()))
        );
    }
    /**
     * TODO
     */
    SandboxScene(const std::string& chunkFileName, const std::string& blocksFileName);

    // Methods
    /**
     * Getter for a voxel in the scene given a position.
     * @param   position    Position of the requested voxel.
     * @return  Voxel object found at that given position.
     */
    Voxel getVoxel(const VoxelPosition& position) const {
        return voxels[position.x][position.y][position.z];
    }
    /**
     * Getter for a voxel in the scene given a position.
     * @note    This version returns a reference!
     * @param   position    Position of the requested voxel.
     * @return  Reference to the Voxel object found at that given position.
     */
    Voxel& getVoxel(const VoxelPosition& position) {
        return voxels[position.x][position.y][position.z];
    }
    /**
     * Setter of a voxel at a given position in the scene.
     * @param   position    Position of the voxel to set.
     * @param   voxel       Voxel to set.
     */
    void setVoxel(const VoxelPosition& position, const Voxel& voxel) {
        voxels[position.x][position.y][position.z] = voxel;
    }
};

#endif//__RAYCAST_SCENE__

