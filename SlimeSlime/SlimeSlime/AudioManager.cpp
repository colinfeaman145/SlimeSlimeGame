#include "AudioManager.hpp"
#include "GameContext.hpp"
#include "Grid.hpp"

bool AudioManager::Initialize(const FMOD_VECTOR& pos) {
    System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, nullptr);
    system->set3DListenerAttributes(0, &pos, nullptr, nullptr, nullptr);
    system->set3DSettings(1.0f, context.grid->GetCellSize() / 10, 2.0f);

    return true;
}

void AudioManager::Process(Vector2 pos, float deltaTime) {
    FMOD_VECTOR loc = { pos.x, 100, pos.y };
    system->set3DListenerAttributes(0, &loc, nullptr, nullptr, nullptr);
    system->update();
    for (auto& c : soundCooldown) {
        c.second -= deltaTime;
    }
}

void AudioManager::Cleanup() {
    for (const auto& [name, group] : groups) {
        group->release();
    }
    for (const auto& [name, sound] : sounds) {
        sound->release();
    }
    system->close();
    system->release();
}

bool AudioManager::LoadSound(const string& filepath, const string& soundName) {
    if (sounds.count(filepath)) return 0;

    Sound* sound = nullptr;
    system->createSound(filepath.c_str(), FMOD_3D | FMOD_3D_LINEARSQUAREROLLOFF, nullptr, &sound);
    sound->set3DMinMaxDistance(context.grid->GetCellSize(), context.grid->GetCellSize() * 25);
    sounds[soundName] = sound;
    soundCooldown[soundName] = 0;
    return 1;
}

/*
* FMOD_VECTOR pos = {x, y, z};
* FMOD_VECTOR vel = {0, 0, 0};
*/
bool AudioManager::PlaySound(const string& soundName, const string& groupName, const FMOD_VECTOR& pos, const FMOD_VECTOR& vel, Vector2 pitchVariance) {
    if (!sounds.contains(soundName)) return 0;//sound not loaded
    if (soundCooldown[soundName] > 0) return 0;//wait for cooldown period

    //set pitch based on variance
    uniform_real_distribution<float> pitchGen(pitchVariance.x, pitchVariance.y);
    SetGroupPitch(groupName, pitchGen(gen));

    Channel* channel = nullptr;
    system->playSound(sounds[soundName], groups[groupName], true, &channel);
    channel->set3DAttributes(&pos, &vel);
    channel->setPaused(false);

    //update cooldown
    soundCooldown[soundName] = SOUND_COOLDOWN;
    return 1;
}

bool AudioManager::AddGroup(const string& name) {
    if (groups.contains(name)) return false;

    ChannelGroup* group = nullptr;
    system->createChannelGroup(name.c_str(), &group);
    groups[name] = group;
    return true;
}

void AudioManager::SetGroupVolume(const string& name, float volume) {
    if (!groups.contains(name)) return;
    groups[name]->setVolume(volume);
}

void AudioManager::SetGroupPitch(const string& name, float pitch) {
    if (!groups.contains(name)) return;
    groups[name]->setPitch(pitch);
}

void AudioManager::SetGroupPaused(const string& name, bool paused) {
    if (!groups.contains(name)) return;
    groups[name]->setPaused(paused);
}

bool AudioManager::IsGroupPaused(const string& name) {
    if (!groups.contains(name)) return 0;
    bool paused = false;
    groups[name]->getPaused(&paused);
    return paused;
}

void AudioManager::StopGroup(const string& name) {
    if (!groups.contains(name)) return;
    groups[name]->stop();
}