//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"
#include "ray_tracing/hittable_list.h"

class RayTracingScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Tracing";

    ~RayTracingScene() override;

    static SceneRef create();

    void renderImage() override;

private:
    RayTracingScene();
    void reset() override;
    void drawSpecificProperty() override;

    void draw() override;

    hittable_list world;

    class camera cam;
    int fov = 20;
    float defocusAngle{10};
    float focusDist{3.4};
};


