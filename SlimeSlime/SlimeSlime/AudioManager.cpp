#include "AudioManager.hpp"

bool AudioManager::Initialize(const FMOD_VECTOR& pos) {
    System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, nullptr);
    listenerPos = pos;
    system->set3DListenerAttributes(0, &pos, nullptr, nullptr, nullptr);

    return true;
}

//TODO pull camera location and update listenerPos
void AudioManager::Update() {
    system->set3DListenerAttributes(0, &listenerPos, nullptr, nullptr, nullptr);
    system->update();
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
    system->createSound(filepath.c_str(), FMOD_DEFAULT, nullptr, &sound);
    sounds[soundName] = sound;
    return 1;
}

/*
* FMOD_VECTOR pos = {x, y, z};
* FMOD_VECTOR vel = {0, 0, 0};
*/
bool AudioManager::PlaySound(const string& soundName, const string& groupName, const FMOD_VECTOR& pos, const FMOD_VECTOR& vel) {
    if (!sounds.contains(soundName)) return -1;//sound not loaded

    Channel* channel = nullptr;
    channel->set3DAttributes(&pos, &vel);
    system->playSound(sounds[soundName], groups[groupName], false, &channel);
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