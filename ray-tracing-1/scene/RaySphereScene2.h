//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RaySphereScene2 : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Sphere2";

    static SceneRef create();

    void renderImage() override;

private:
    RaySphereScene2();
    void reset() override;

    class camera cam;
};


