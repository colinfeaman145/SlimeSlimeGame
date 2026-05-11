#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP
#include <fmod.hpp>
#include <map>
#include <string>
#include <vector>
#include "Vector2.hpp"

#define SOUND_COOLDOWN 0.1

using namespace FMOD;
using namespace std;
class AudioManager {
public:
    AudioManager() = default;
    bool Initialize(const FMOD_VECTOR& listenerPos);
    void Process(Vector2 pos, float deltaTime);
    void Cleanup();

    bool LoadSound(const string& filepath, const string& soundName);
    bool PlaySound(const string& soundName, const string& groupName, const FMOD_VECTOR& pos, const FMOD_VECTOR& vel, Vector2 pitchVariance);
        
    //group control
    bool AddGroup(const string& name);
    void SetGroupVolume(const string& name, float volume);
    void SetGroupPitch(const string& name, float pitch);
    void SetGroupPaused(const string& name, bool paused);
    bool IsGroupPaused(const string& name);
    void StopGroup(const string& name);

    bool LoadMusicTrack(const string& name, const string& path);
    void PlayMusic(const string& soundName);
    void ProcessMusic();
    bool IsSoundPlaying();

    System* GetSystem() { return system; }

private:
    System* system = nullptr;
    map<string, Sound*> sounds;
    map<string, float> soundCooldown;
    map<string, ChannelGroup*> groups;
    FMOD_VECTOR listenerPos;
    bool ambientPlaying;
    bool musicPlaying;
    vector<string> musicTracks;
    string currentTrack;
    Channel* musicChannel = nullptr;
};

#endif