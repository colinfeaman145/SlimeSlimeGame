#ifndef FLOWFIELD_HPP
#define FLOWFIELD_HPP

#include "GridCoord.hpp"
#include "Vector2.hpp"
#include <vector>

struct FlowField {
    std::vector<Vector2> vectors;
    std::vector<bool>    reached;
    GridCoord            target = { -1, -1 };
    bool                 dirty = true;

    void Reset(int gridWidth, int gridHeight) {
        int total = gridWidth * gridHeight;
        vectors.assign(total, Vector2(0.f, 0.f));
        reached.assign(total, false);
    }

    bool HasVector(int idx) const {
        return idx >= 0 && idx < (int)reached.size() && reached[idx];
    }
};

#endif