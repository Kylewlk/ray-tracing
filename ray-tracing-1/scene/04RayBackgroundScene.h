//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/rtweekend.h"

class RayBackgroundScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Background";

    static SceneRef create();

    void renderImage() override;

private:
    RayBackgroundScene();
    void reset() override;

    color rayColor(const ray& ray);

};


