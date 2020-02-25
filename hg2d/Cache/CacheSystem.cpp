#include "CacheSystem.hpp"
#include "../Renderer/RenderSystem.hpp"
#include "../Sound/SoundSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "hd/Core/StringUtils.hpp"
#include "hd/Core/Log.hpp"

namespace hg2d {

CacheSystem::CacheSystem(Engine &engine) : AEngineObject(engine) {
}

void CacheSystem::onInitialize() {
}

void CacheSystem::onShutdown() {
    for (auto &texture : mTextures) {
        mRenderSystem.destroyTexture(texture.second);
    }
    for (auto &texture : mColorTextures) {
        mRenderSystem.destroyTexture(texture.second);
    }
    for (auto &font : mFonts) {
        mGUISystem.destroyFont(font.second);
    }
    for (auto &soundBuffer : mSoundBuffers) {
        mSoundSystem.destroySound(soundBuffer.second);
    }
    for (auto &musicBuffer : mMusicBuffers) {
        mSoundSystem.destroyMusic(musicBuffer.second);
    }
}

Texture *CacheSystem::loadTexture(const std::string &filename) {
    if (!filename.empty()) {
        if (mTextures.count(filename) == 0) {
            Texture *texture = mRenderSystem.createTextureFromFile(filename);
            mTextures.insert(std::make_pair(filename, texture));
            return texture;
        }
        else {
            return mTextures.at(filename);
        }
    }
    else {
        LOG_F(FATAL, "Failed to load texture. Filename is empty");
        return nullptr;
    }
}

Texture *CacheSystem::loadTexture(const hd::Color4 &color) {
    std::string key = fmt::format("{} {} {} {}", color.r, color.g, color.b, color.a);
    if (mColorTextures.count(key) == 0) {
        Texture *texture = mRenderSystem.createTextureFromColor(color);
        mColorTextures.insert(std::make_pair(key, texture));
        return texture;
    }
    else {
        return mColorTextures.at(key);
    }
}

Font *CacheSystem::loadFont(const std::string &filename, uint32_t fontSize) {
    if (!filename.empty()) {
        std::string name = filename + "@" + std::to_string(fontSize);
        if (mFonts.count(name) == 0) {
            Font *font = mGUISystem.createFontFromFile(filename, fontSize);
            mFonts.insert(std::make_pair(name, font));
            return font;
        }
        else {
            return mFonts.at(name);
        }
    }
    else {
        LOG_F(FATAL, "Failed to load font. Filename is empty");
        return nullptr;
    }
}

SoundBuffer *CacheSystem::loadSound(const std::string &filename) {
    if (!filename.empty()) {
        if (mSoundBuffers.count(filename) == 0) {
            SoundBuffer *soundBuffer = mSoundSystem.createSoundFromFile(filename);
            mSoundBuffers.insert(std::make_pair(filename, soundBuffer));
            return soundBuffer;
        }
        else {
            return mSoundBuffers.at(filename);
        }
    }
    else {
        LOG_F(FATAL, "Failed to load sound. Filename is empty");
        return nullptr;
    }
}

MusicBuffer *CacheSystem::loadMusic(const std::string &filename) {
    if (!filename.empty()) {
        if (mMusicBuffers.count(filename) == 0) {
            MusicBuffer *musicBuffer = mSoundSystem.createMusicFromFile(filename);
            mMusicBuffers.insert(std::make_pair(filename, musicBuffer));
            return musicBuffer;
        }
        else {
            return mMusicBuffers.at(filename);
        }
    }
    else {
        LOG_F(FATAL, "Failed to load music. Filename is empty");
        return nullptr;
    }
}

}
