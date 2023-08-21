//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RayTracingScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Tracing";

    static SceneRef create();

    void renderImage() override;

private:
    RayTracingScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;
    int fov = 20;
    float defocusAngle{10};
    float focusDist{3.4};
};


