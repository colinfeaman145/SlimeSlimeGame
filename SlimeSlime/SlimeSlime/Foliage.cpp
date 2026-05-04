#include "Foliage.hpp"

void Foliage::HandleCollision(Collidable* other, Vector2 penetration, GameContext& context) {
	if (Structure* s = dynamic_cast<Structure*>(other)) {//if overlapping with structure(or nature) delete itself
		Break();
	}
	if (AttackCone* c = dynamic_cast<AttackCone*>(other)) {//if attacked, delete
		Break();
	}
	//rest of object will be deleted in GridCell::DrawNature
}