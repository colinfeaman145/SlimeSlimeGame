#include "Foliage.hpp"

void Foliage::HandleCollision(Collidable* other, Vector2 penetration) {
	if (other->GetCollidableType() == CollidableType::STRUCTURE) {//if overlapping with structure delete itself
		Break();
	}
	//rest of object will be deleted in GridCell::DrawNature
}