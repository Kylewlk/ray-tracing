//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RayViewingScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Positionable Camera";

    static SceneRef create();

    void renderImage() override;

private:
    RayViewingScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;
};


