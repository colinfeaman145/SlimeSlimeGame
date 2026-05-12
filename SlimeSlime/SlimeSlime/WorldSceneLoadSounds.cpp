#include "WorldScene.hpp"

void WorldScene::LoadSounds() {

	context.am->AddGroup("Default");

	//sword
	context.am->LoadSound("../../assets/sounds/sword_swing.mp3", "SwordSwing");

	//nature
	context.am->LoadSound("../../assets/sounds/break.mp3", "Break");
	context.am->LoadSound("../../assets/sounds/tree_hit.wav", "WoodHit");
	context.am->LoadSound("../../assets/sounds/bush_hit.wav", "BushHit");
	context.am->LoadSound("../../assets/sounds/rock_hit.wav", "RockHit");

	//build
	context.am->LoadSound("../../assets/sounds/build_mode.wav", "BuildMode");
	context.am->LoadSound("../../assets/sounds/build_place.wav", "BuildPlace");
	context.am->LoadSound("../../assets/sounds/cantPlace.mp3", "CantPlace");
	context.am->LoadSound("../../assets/sounds/breakStructure.mp3", "BreakStructure");

	//traps
	context.am->LoadSound("../../assets/sounds/boost_pad.mp3", "PushTrap");
	context.am->LoadSound("../../assets/sounds/flame_trap.mp3", "FlameTrap");
	context.am->LoadSound("../../assets/sounds/ice_trap.mp3", "FreezeTrap");
	context.am->LoadSound("../../assets/sounds/spike_trap.mp3", "SpikeTrap");

	//slime
	context.am->LoadSound("../../assets/sounds/slime_hit1.wav", "SlimeHit1");
	context.am->LoadSound("../../assets/sounds/slime_hit2.wav", "SlimeHit2");
	context.am->LoadSound("../../assets/sounds/slime_hit3.wav", "SlimeHit3");

	//background
	context.am->LoadMusicTrack("../../assets/sounds/ambient.mp3", "Ambient");
	context.am->LoadMusicTrack("../../assets/sounds/background_music1.mp3", "Song1");
	context.am->LoadMusicTrack("../../assets/sounds/background_music2.mp3", "Song2");
	context.am->LoadMusicTrack("../../assets/sounds/background_music2.mp3", "Song3");

	//pickup
	context.am->LoadSound("../../assets/sounds/item_pickup.wav", "Pickup");

	//explosion
	context.am->LoadSound("../../assets/sounds/explosion.mp3", "Explosion");
	context.am->LoadSound("../../assets/sounds/explosion_prepare.mp3", "ExplosionPrepare");

	//upgrade
	context.am->LoadSound("../../assets/sounds/upgrade.mp3", "Upgrade");

}