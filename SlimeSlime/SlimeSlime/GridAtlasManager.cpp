#include "Grid.hpp"


Structure* Grid::GetAtlas() {
	if (!atlas) return nullptr;
	else return atlas;
}

void Grid::SetAtlas(Structure* a) {
	atlas = a;
}

void Grid::MoveAtlas() {

}