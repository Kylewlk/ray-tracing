//
// Created by wlk12 on 2023/8/6.
//

#pragma once

#include "Define.h"
#include "common/MathHelp.h"

class Scene
{
public:
    static constexpr const char* PropertyWindow = "Scene-Property";

    virtual ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(Scene&) = delete;

    [[nodiscard]] const std::string& getName() const { return this->name; }
    [[nodiscard]] TextureRef getColorTexture();

    virtual void resize(int width, int height);
    virtual void render();

protected:
    Scene(const char* name, int width, int height);

    virtual void draw();
    virtual void drawProperty();

    std::string name;
    int width{0};
    int height{0};
    math::Vec4 clearColor{0.0f, 0.0f, 0.0f, 1.0f};

    FrameBufferRef fbDraw; // multi-sample
    FrameBufferRef fbResolved;

};


