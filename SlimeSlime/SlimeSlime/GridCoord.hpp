#ifndef GRIDCOORD_HPP
#define GRIDCOORD_HPP

struct GridCoord {
    int col;
    int row;

    bool operator==(const GridCoord& other) const {
        return col == other.col && row == other.row;
    }
};

enum class EdgeDirection {
    NORTH,
    WEST
};

#endif