//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"

class RayDiffuseScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Diffuse";

    static SceneRef create();

    void renderImage() override;

private:
    RayDiffuseScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;
    bool gamma{true};
};


