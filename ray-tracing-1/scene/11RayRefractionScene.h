//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"
#include "ray_tracing/hittable_list.h"

class RayRefractionScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Refraction";

    ~RayRefractionScene() override;

    static SceneRef create();

    void renderImage() override;

private:
    RayRefractionScene();
    void reset() override;
    void drawSpecificProperty() override;
    void draw() override;

    hittable_list world;

    class camera cam;
};


