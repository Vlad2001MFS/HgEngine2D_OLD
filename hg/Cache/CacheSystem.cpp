#include "CacheSystem.hpp"
#include "../Renderer/RenderSystem2D.hpp"
#include "../Sound/SoundSystem.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Core/Log.hpp"

namespace hg {

void CacheSystem::initialize() {
}

void CacheSystem::shutdown() {
    for (auto &texture : mTextures) {
        getRenderSystem2D().destroyTexture(texture.second);
    }
    for (auto &texture : mColorTextures) {
        getRenderSystem2D().destroyTexture(texture.second);
    }
    for (auto &soundBuffer : mSoundBuffers) {
        getSoundSystem().destroySound(soundBuffer.second);
    }
    for (auto &musicBuffer : mMusicBuffers) {
        getSoundSystem().destroyMusic(musicBuffer.second);
    }
}

Texture *CacheSystem::loadTexture(const std::string &filename) {
    if (!filename.empty()) {
        if (mTextures.count(filename) == 0) {
            Texture *texture = getRenderSystem2D().createTextureFromFile(filename);
            mTextures.insert(std::make_pair(filename, texture));
            return texture;
        }
        else {
            return mTextures.at(filename);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load texture. Filename is empty");
        return nullptr;
    }
}

Texture *CacheSystem::loadTexture(const hd::Color4 &color) {
    std::string key = fmt::format("{} {} {} {}", color.r, color.g, color.b, color.a);
    if (mColorTextures.count(key) == 0) {
        Texture *texture = getRenderSystem2D().createTextureFromColor(color);
        mColorTextures.insert(std::make_pair(key, texture));
        return texture;
    }
    else {
        return mColorTextures.at(key);
    }
}

SoundBuffer *CacheSystem::loadSound(const std::string &filename) {
    if (!filename.empty()) {
        if (mSoundBuffers.count(filename) == 0) {
            SoundBuffer *soundBuffer = getSoundSystem().createSoundFromFile(filename);
            mSoundBuffers.insert(std::make_pair(filename, soundBuffer));
            return soundBuffer;
        }
        else {
            return mSoundBuffers.at(filename);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load sound. Filename is empty");
        return nullptr;
    }
}

MusicBuffer *CacheSystem::loadMusic(const std::string &filename) {
    if (!filename.empty()) {
        if (mMusicBuffers.count(filename) == 0) {
            MusicBuffer *musicBuffer = getSoundSystem().createMusicFromFile(filename);
            mMusicBuffers.insert(std::make_pair(filename, musicBuffer));
            return musicBuffer;
        }
        else {
            return mMusicBuffers.at(filename);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load music. Filename is empty");
        return nullptr;
    }
}

}

