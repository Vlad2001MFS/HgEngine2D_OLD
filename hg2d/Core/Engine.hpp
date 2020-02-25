#pragma once
#include "hd/Core/FPSCounter.hpp"
#include "hd/Window/Window.hpp"
#include <memory>

namespace hg2d {

struct WindowCreateInfo final {
    std::string title = "HgEngine2D Application";
    glm::ivec2 size = glm::ivec2(640, 480);
    bool fullscreen = false;
};

struct SoundSystemCreateInfo final {
    uint32_t freq = 22050;
    uint32_t chunkSize = 4096;
    bool isStereo = true;
};

struct GUISystemCreateInfo final {
    std::string fontPath = "font.ttf";
    uint32_t fontSize = 16;
    bool monospacedFont = true;
    hd::Color4 fontColor = hd::Color4::White;
    std::string buttonTexturePath = "gui/button.png";
    std::string hoveredButtonTexturePath = "gui/hoveredButton.png";
    std::string clickedButtonTexturePath = "gui/clickedButton.png";
    int alignSpaceX = 10;
    int alignSpaceY = 10;
};

struct EngineCreateInfo final {
    WindowCreateInfo window;
    SoundSystemCreateInfo sound;
    GUISystemCreateInfo gui;
};

class Engine final : public hd::Singleton<Engine> {
public:
    void initialize(const EngineCreateInfo &createInfo);
    void shutdown();
    void run();
    void close();

    bool isKeyDown(hd::KeyCode key) const;
    bool isKeyDown(hd::MouseButton button) const;
    const EngineCreateInfo &getCreateInfo() const;
    hd::Window &getWindow();
    uint32_t getFps() const;
    float getFrameTime() const;

private:
    EngineCreateInfo mCreateInfo;
    hd::Window mWindow;
    hd::FPSCounter mFPSCounter;
};

inline Engine &getEngine() {
    return Engine::get();
}

}