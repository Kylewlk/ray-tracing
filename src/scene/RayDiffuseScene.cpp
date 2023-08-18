//
// Created by wlk12 on 2023/8/7.
//

#include "RayDiffuseScene.h"
#include "common/Texture.h"

#include "ray_tracing/rtweekend.h"
#include "ray_tracing/hittable_list.h"
#include "ray_tracing/sphere.h"


RayDiffuseScene::RayDiffuseScene()
    : BaseScene(ID, 0, 0)
{
    this->gamma = true;
    this->cam.type = camera::diffuse_gamma;
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplerPerPixel = 10;
    RayDiffuseScene::renderImage();
}

SceneRef RayDiffuseScene::create()
{
    struct enable_make_shared : public RayDiffuseScene
    {
        enable_make_shared() : RayDiffuseScene() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RayDiffuseScene::renderImage()
{
    this->imageCurrentWidth = this->imageWidth;
    this->imageCurrentHeight = this->imageHeight;

    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    this->cam.render(world, this->aspectRatio, imageWidth, samplerPerPixel, imagePixels);

    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RayDiffuseScene::reset()
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplerPerPixel = 10;
    BaseScene::reset();
}

void RayDiffuseScene::drawSpecificProperty()
{
    ImGui::SliderInt("Sampler Count", &samplerPerPixel, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Ray Max Depth", &this->cam.max_depth, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
    if (ImGui::Checkbox("Gamma", &this->gamma))
    {
        this->cam.type = gamma ? camera::diffuse_gamma : camera::diffuse;
    }
}
