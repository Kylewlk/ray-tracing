//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RayRefractionScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Refraction";

    static SceneRef create();

    void renderImage() override;

private:
    RayRefractionScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;
};


