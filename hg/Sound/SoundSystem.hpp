#pragma once
#include "hd/Core/Common.hpp"
#include "hd/Core/Handle.hpp"
#include <vector>
#include <string>

namespace hg {

struct SoundBuffer;
struct MusicBuffer;

using SoundChannel = hd::Handle<int, struct TAG_SoundChannel, -1>;

class SoundSystem final : public hd::Singleton<SoundSystem> {
public:
    void initialize();
    void shutdown();

    SoundBuffer *createSoundFromFile(const std::string &filename);
    MusicBuffer *createMusicFromFile(const std::string &filename);
    void destroySound(SoundBuffer *&soundBuffer);
    void destroyMusic(MusicBuffer *&musicBuffer);

    SoundChannel playChannel(const SoundBuffer *buffer, int loops = 0);
    SoundChannel playChannel(const SoundChannel &channel, const SoundBuffer *buffer, int loops = 0);
    void pauseChannel(const SoundChannel &channel);
    void resumeChannel(const SoundChannel &channel);
    void stopChannel(const SoundChannel &channel);
    bool isChannelPlaying(const SoundChannel &channel) const;
    bool isChannelPaused(const SoundChannel &channel) const;

    void playMusic(const MusicBuffer *buffer, int loops);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();
    bool isMusicPlaying() const;
    bool isMusicPaused() const;

private:
    void mDestroySound(SoundBuffer *&buffer);
    void mDestroyMusic(MusicBuffer *&buffer);

    std::vector<SoundBuffer*> mCreatedSoundBuffers;
    std::vector<MusicBuffer*> mCreatedMusicBuffers;
};

inline SoundSystem &getSoundSystem() {
    return SoundSystem::get();
}

}
