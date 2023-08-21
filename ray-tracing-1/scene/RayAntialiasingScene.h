//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RayAntialiasingScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Antialiasing";

    static SceneRef create();

    void renderImage() override;

private:
    RayAntialiasingScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;
};


