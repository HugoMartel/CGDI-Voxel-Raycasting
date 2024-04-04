#include <vector>
#include <cassert>
#include "voxel.cpp"

template<typename T>
using Lattice3D = std::vector<std::vector<std::vector<T>>>;

class SandboxScene {
private:
    Lattice3D<Voxel> voxels;

public:
    SandboxScene(int width, int height, int depth) {
        voxels = Lattice3D<Voxel>(width, std::vector<std::vector<Voxel>>(
            height, std::vector<Voxel>(depth, Voxel()))
        );
    }

    Voxel getVoxel(VoxelPosition& position) {
        return voxels[position.x][position.y][position.z];
    };
    void setVoxel(VoxelPosition& position, Voxel& voxel) {
        voxels[position.x][position.y][position.z] = voxel;
    }
};
