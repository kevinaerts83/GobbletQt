#ifndef GOBBLER3D_H
#define GOBBLER3D_H
#include "shape3d.h"
#include <memory>
#include <unordered_map>

class Gobbler3d : public Shape3d
{
public:
    Gobbler3d(double size);

    /// Returns a shared Gobbler3d model for the given size.
    /// Models are created once and reused across all gobblers of the same size.
    static std::shared_ptr<Gobbler3d> sharedModel(double size);

private:
    static std::unordered_map<int, std::shared_ptr<Gobbler3d>> s_models;
};

#endif // GOBBLER3D_H
