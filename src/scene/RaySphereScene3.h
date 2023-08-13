//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/hittable.h"

class RaySphereScene3 : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Sphere3";

    static SceneRef create();

    void renderImage() override;

private:
    RaySphereScene3();
    void reset() override;

    color rayColor(const ray& ray, const hittable& world);
};


