#pragma once
#include "../Core/JSONObject.hpp"
#include "../Core/AEngineObject.hpp"
#include "hd/Core/Handle.hpp"
#include "hd/Window/WindowEvent.hpp"
#include <map>
#include <vector>
#include <typeinfo>
#include <functional>

namespace hg2d {

using HEntity = hd::Handle<uint64_t, struct TAG_HEntity, UINT64_MAX>;

class AECSComponent : public AEngineObject {
public:
    AECSComponent(Engine &engine);
    virtual ~AECSComponent() = default;

    virtual void onSaveLoad(JSONObject &json, bool isLoad);
};

class AECSSystem : public AEngineObject {
public:
    AECSSystem(Engine &engine);
    virtual ~AECSSystem() = default;

    virtual void onInitialize();
    virtual void onShutdown();
    virtual void onEvent(const hd::WindowEvent &event);
    virtual void onCreateComponent(AECSComponent *component, uint64_t typeHash, const HEntity &entity);
    virtual void onDestroyComponent(AECSComponent *component, uint64_t typeHash, const HEntity &entity);
    virtual void onSaveLoad(JSONObject &json, bool isLoad);
    virtual void onClear();
    virtual void onFixedUpdate();
    virtual void onUpdate();
    virtual void onDraw();
};

class SceneSystem final : public AEngineObject{
public:
    explicit SceneSystem(Engine &engine);

    void onInitialize();
    void onShutdown();
    void onEvent(const hd::WindowEvent &event);
    void onFixedUpdate();
    void onUpdate();
    void onDraw();

    void setUpdateEnabled(bool enabled);
    void setDrawEnabled(bool enabled);

    void clear();
    void save(const std::string &name);
    void load(const std::string &name);

    HEntity createEntity();
    void destroyEntity(HEntity &handle);

    template<typename T>
    void registerComponentType() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mComponentTypes.count(typeHash) == 0) {
            mComponentTypes.insert(std::make_pair(typeHash, std::make_pair(typeName, [&]() { return new T(mEngine); })));
        }
    }

    AECSComponent *createComponent(HEntity &handle, uint64_t typeHash);

    template<typename T>
    T *createComponent(HEntity &handle) {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        return static_cast<T*>(mCreateComponent(handle, typeHash, typeName));
    }

    template<typename T>
    void destroyComponent(HEntity &handle) {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (handle && handle.value < mEntities.size() && mComponentsMap.count(typeHash) != 0) {
            AECSComponent *&component = mComponentsMap.at(typeHash).at(handle.value);
            mDestroyComponent(component, typeHash, handle);
        }
    }

    template<typename T>
    T *getComponent(const HEntity &handle) {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (handle && handle.value < mEntities.size() && mComponentsMap.count(typeHash) != 0) {
            return static_cast<T*>(mComponentsMap.at(typeHash).at(handle.value));
        }
        else {
            return nullptr;
        }
    }

    template<typename T>
    const std::vector<T*> &getComponents() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mComponentsMap.count(typeHash) == 0) {
            mComponentsMap.insert(std::make_pair(typeHash, std::vector<AECSComponent*>(mEntities.size(), nullptr)));
        }
        return reinterpret_cast<const std::vector<T*>&>(mComponentsMap.at(typeHash));
    }

    template<typename T>
    T *createSystem() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mSystems.count(typeHash) == 0) {
            T *system = new T(mEngine);
            mSystems.insert(std::make_pair(typeHash, std::make_pair(typeName, system)));
            system->onInitialize();
            return system;
        }
        else {
            return static_cast<T*>(mSystems.at(typeHash).second);
        }
    }

    template<typename T>
    void destroySystem() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mSystems.count(typeHash) != 0) {
            mDestroySystem(mSystems.at(typeHash).second);
            mSystems.erase(typeHash);
        }
    }

    template<typename T>
    T *getSystem() {
        const std::type_info &typeInfo = typeid(T);
        const char *typeName = typeInfo.name();
        uint64_t typeHash = typeInfo.hash_code();
        if (mSystems.count(typeHash) != 0) {
            return static_cast<T*>(mSystems.at(typeHash).second);
        }
        else {
            return nullptr;
        }
    }

    const std::vector<HEntity> &getEntities() const;
    const std::map<uint64_t, std::vector<AECSComponent*>> &getComponentsMap() const;
    const std::map<uint64_t, std::pair<std::string, AECSSystem*>> &getSystems() const;

private:
    HEntity mCreateEntity(uint64_t idx);
    AECSComponent *mCreateComponent(HEntity& handle, uint64_t typeHash, const std::string &typeName);
    void mDestroyComponent(AECSComponent *&component, uint64_t typeHash, const HEntity &entity);
    void mDestroySystem(AECSSystem *&system);

    bool mIsUpdateEnabled;
    bool mIsDrawEnabled;

    std::vector<HEntity> mEntities;
    std::map<uint64_t, std::vector<AECSComponent*>> mComponentsMap;
    std::map<uint64_t, std::pair<std::string, AECSSystem*>> mSystems;

    std::map<uint64_t, std::pair<std::string, std::function<AECSComponent*()>>> mComponentTypes;
};

}