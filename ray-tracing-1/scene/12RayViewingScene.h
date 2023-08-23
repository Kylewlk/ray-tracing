//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"
#include "ray_tracing/hittable_list.h"

class RayViewingScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Positionable Camera";

    ~RayViewingScene() override;

    static SceneRef create();

    void renderImage() override;

private:
    RayViewingScene();
    void reset() override;
    void drawSpecificProperty() override;
    void draw() override;

    hittable_list world;

    class camera cam;
    int fov = 45;
};


