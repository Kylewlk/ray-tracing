//
// Created by wlk12 on 2023/8/7.
//

#include "11RayRefractionScene.h"
#include "common/Texture.h"

#include "ray_tracing/hittable_list.h"
#include "ray_tracing/sphere.h"


RayRefractionScene::RayRefractionScene()
    : BaseScene(ID, 0, 0)
{
    this->cam.type = camera::material;
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 20;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   -0.4, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    RayRefractionScene::renderImage();
}

RayRefractionScene::~RayRefractionScene()
{
    this->isRendering = false;
    this->running = false;
    if (this->renderResult.valid())
    {
        this->renderResult.get();
    }
}

SceneRef RayRefractionScene::create()
{
    struct enable_make_shared : public RayRefractionScene
    {
        enable_make_shared() : RayRefractionScene() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RayRefractionScene::renderImage()
{
    if (this->isRendering)
    {
        this->isRendering = false;
    }
    else
    {
        BaseScene::reset();

        this->imageCurrentWidth = this->imageWidth;
        this->imageCurrentHeight = this->imageHeight;
        this->isRendering = true;
    }
}

void RayRefractionScene::reset()
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 20;
    BaseScene::reset();
}

void RayRefractionScene::draw()
{
    if (!this->renderResult.valid())
    {
        if (this->isRendering)
        {
            if (this->sampleCurrentCount < this->samplePerPixel)
            {
                this->renderResult = this->cam.renderAsync(world, this->aspectRatio, imageWidth, renderingPixel);
            }
            else
            {
                this->isRendering = false;
            }
        }
    }
    else
    {
        if(this->renderResult.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
        {
            if (this->renderResult.get())
            {
                this->updateImage();
            }
        }
    }

    BaseScene::draw();
}

void RayRefractionScene::drawSpecificProperty()
{
    ImGui::SliderInt("Sampler Count", &samplePerPixel, 1, 500, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Ray Max Depth", &this->cam.max_depth, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
}
