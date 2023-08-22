//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/camera.h"
#include <future>

class RayAntialiasingScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Antialiasing";

    ~RayAntialiasingScene() override;

    static SceneRef create();

    void renderImage() override;

private:
    RayAntialiasingScene();
    void reset() override;
    void drawSpecificProperty() override;

    class camera cam;

    static constexpr int threadCount = 6;
    std::future<bool> renderResult[threadCount];
    std::atomic<bool> rendering[threadCount];
};


