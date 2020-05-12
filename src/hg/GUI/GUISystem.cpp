#include "GUISystem.hpp"
#include "../Graphics/RenderDevice.hpp"
#include "../Core/Engine.hpp"
#include "hd/Core/Log.hpp"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_sdl.h"
#include "../../imgui/imgui_impl_opengl3.h"

namespace hg {

GUISystem::GUISystem() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(getEngine().getWindow(), getEngine().getGLContext());
    ImGui_ImplOpenGL3_Init();

    mSkin.font = loadFont("font.ttf", 24);
    mSkin.font->setHinting(FontHinting::Mono);
    mSkin.fontColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mSkin.buttonTexture = getRenderDevice().loadTexture2D("ui/button.png");
    mSkin.hoveredButtonTexture = getRenderDevice().loadTexture2D("ui/hoveredButton.png");
    mSkin.clickedButtonTexture = getRenderDevice().loadTexture2D("ui/clickedButton.png");
    mSkin.alignSpaceX = 10;
    mSkin.alignSpaceY = 10;
}

GUISystem::~GUISystem() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUISystem::onEvent(const WindowEvent &event) {
    ImGui_ImplSDL2_ProcessEvent(&event.sdlEvent);
}

void GUISystem::onUpdate(float dt) {
    static bool firstUpdate = true;
    if (!firstUpdate) {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    firstUpdate = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(getEngine().getWindow());
    ImGui::NewFrame();
}

FontPtr GUISystem::loadFont(const std::string &path, uint32_t size) {
    if (!path.empty()) {
        hd::StringHash pathHash = hd::StringHash(fmt::format("{}@{}", path, size));
        if (mFontsDB.count(pathHash) == 0) {
            FontPtr font = Font::createFromFile(path, size);
            mFontsDB.insert(std::make_pair(pathHash, font));
            return font;
        }
        else {
            return mFontsDB.at(pathHash);
        }
    }
    else {
        HD_LOG_FATAL("Failed to load font. Path is empty");
        return FontPtr();
    }
}

const GUISkin &GUISystem::getSkin() const {
    return mSkin;
}

GUISystem &getGUISystem() {
    return getEngine().getGUISystem();
}

}
