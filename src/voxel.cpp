#include <vector>

struct VoxelPosition {
public:
    int x, y, z;
    VoxelPosition(int x, int y, int z) : x(x), y(y), z(z) {}
    bool operator==(VoxelPosition pos) {
        return (x == pos.x && y == pos.y && z == pos.z);
    }
};

struct AABB {
private:
    double minX, minY, minZ;
    double maxX, maxY, maxZ;
    AABB(
        double minX, double minY, double minZ,
        double maxX, double maxY, double maxZ
    ) : minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {}
};

class Voxel {
private:
    std::vector<AABB> contents;

public:
    Voxel() : contents(std::vector<AABB>()) {}

    Voxel(std::vector<AABB> contents) : contents(contents) {}

    std::vector<AABB> getContents() {
        return contents;
    }

    void addAABB(AABB box) {
        contents.push_back(box);
    }
};
