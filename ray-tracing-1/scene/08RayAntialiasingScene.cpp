//
// Created by wlk12 on 2023/8/7.
//

#include "08RayAntialiasingScene.h"
#include "common/Texture.h"
#include "camera/Camera2D.h"
#include "ray_tracing/rtweekend.h"
#include "ray_tracing/hittable_list.h"
#include "ray_tracing/sphere.h"


RayAntialiasingScene::RayAntialiasingScene()
    : BaseScene(ID, 0, 0)
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 10;
    RayAntialiasingScene::renderImage();
}

SceneRef RayAntialiasingScene::create()
{
    struct enable_make_shared : public RayAntialiasingScene
    {
        enable_make_shared() : RayAntialiasingScene() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RayAntialiasingScene::renderImage()
{
    this->imageCurrentWidth = this->imageWidth;
    this->imageCurrentHeight = this->imageHeight;
    this->sampleCurrentCount = this->samplePerPixel;

    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    this->cam.render(world, this->aspectRatio, imageWidth, samplePerPixel, imagePixels);

    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RayAntialiasingScene::reset()
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 10;
    this->camera->resetView();
}

void RayAntialiasingScene::drawSpecificProperty()
{
    ImGui::SliderInt("Sampler Count", &samplePerPixel, 1, 500, "%d", ImGuiSliderFlags_AlwaysClamp);
}
