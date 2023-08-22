//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RayDefocusBlurScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Defocus Blur";

    static SceneRef create();

    void renderImage() override;

private:
    RayDefocusBlurScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;
    int fov = 20;
    float defocusAngle{10};
    float focusDist{3.4};
};


