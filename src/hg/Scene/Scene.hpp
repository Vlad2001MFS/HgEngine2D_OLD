#pragma once
#include "GameObject.hpp"

namespace hg {

class Scene : public GameObject {
public:
    void onEvent(const WindowEvent &event);
    void onFixedUpdate();
    void onUpdate(float dt);

    void clear();
    void save(const std::string &path);
    void load(const std::string &path);

private:
    static std::string mGetFullPath(const std::string &path);
};

Scene &getScene();

}