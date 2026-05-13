#include "Nature.hpp"

void Nature::Initialize(Sprite* spr, NatureType t, bool canCollide) {
	Entity::Initialize(Vector2(), Vector2(), spr);//real position set in gridCell PlaceNature
	SetCanCollide(canCollide);

	sprite->SetDrawLayer(RenderLayer::NATURE, Entity::position.y);
	type = t;
	collideType = CollidableType::NATURE;
	//health set in nature.hpp
}

void Nature::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
	pe->Draw(renderer);
	processedThisFrame = false;//called after process
}

void Nature::Process(float deltaTime) {
	if (processedThisFrame) return;
	processedThisFrame = true;

	Entity::Process(deltaTime);
	pe->Process(deltaTime);
}

void Nature::Damage(float amount) {
	health -= amount;
	SetFlash(true);
	healthBar->SetValues(health, maxHealth);

	if(health <= 0)
		Break(); //no drops unless broke by player

	//aesthetics!!
	if(type != NatureType::FOLIAGE) pe->Burst(5);
	switch (type) {
		case(NatureType::TREE):
		case(NatureType::STUMP):
			context.am->PlaySound("WoodHit", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
			break;
		case(NatureType::BUSH):
			context.am->PlaySound("BushHit", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
			break;
		case(NatureType::ROCK):
			context.am->PlaySound("RockHit", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
			break;
	}
}

void Nature::Break() {
	SetDead();

	//play sound
	if (type == NatureType::TREE || type == NatureType::ROCK ) {
		context.am->PlaySound("Break", "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
	}
}

ResourceType Nature::GetDropType() const {

	switch (type) {
		case(NatureType::TREE):
			return ResourceType::WOOD;
		case(NatureType::BUSH):
			return ResourceType::WOOD;
		case(NatureType::STUMP):
			return ResourceType::WOOD;
		case(NatureType::ROCK):
			return ResourceType::STONE;
		case(NatureType::FOLIAGE):
			return ResourceType::COIN;
	}
}

void Nature::SpawnDrops() {
	//spawn drops
	ResourceDrop drops = ResourceDrop();
	drops.amount = GetDropAmount();
	drops.type = GetDropType();
	drops.spawnerPosition = GetPosition();
	drops.spawnerSize = GetSprite()->GetDrawSize();
	context.grid->SpawnDrops(drops);
}

int Nature::GetDropAmount() const {
	if (type == NatureType::FOLIAGE) {
		uniform_int_distribution<int> giveCoin(1, 50);
		return (giveCoin(gen) == 4);
	}
	return (((double)maxHealth / 10) * dropVariance(gen));
}

void Nature::SetPosition(Vector2 pos) {
	Entity::SetPosition(pos);
	Entity::sprite->SetDrawLayer(RenderLayer::NATURE, position.y + Entity::sprite->GetHeight());
}

Vector2 Nature::GetPosition() const {
	return position;
}

void Nature::HandleCollision(Collidable* other, Vector2 penetration) {
	return;
	//if (Nature* n = dynamic_cast<Nature*>(other)) {//if its another nature
	//	SetPosition(position - penetration);
	//}
}
