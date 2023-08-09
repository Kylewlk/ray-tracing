//
// Created by wlk12 on 2023/8/7.
//

#pragma once
#include "BaseScene.h"
#include "ray_tracing/vec3.h"
#include "ray_tracing/ray.h"

class RaySphereScene : public BaseScene
{
public:
    static constexpr const char* ID = "Ray Sphere";

    static SceneRef create();

    void renderImage() override;

private:
    RaySphereScene();
    void reset() override;

    color rayColor(const ray& ray);
    double hitSphere(const point3& center, double radius, const ray& r);
};


