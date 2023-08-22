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
    this->samplePerPixel = 10;
    RayRefractionScene::renderImage();
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
    this->imageCurrentWidth = this->imageWidth;
    this->imageCurrentHeight = this->imageHeight;

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   -0.4, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    this->cam.render(world, this->aspectRatio, imageWidth, samplePerPixel, imagePixels);

    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RayRefractionScene::reset()
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 10;
    BaseScene::reset();
}

void RayRefractionScene::drawSpecificProperty()
{
    ImGui::SliderInt("Sampler Count", &samplePerPixel, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Ray Max Depth", &this->cam.max_depth, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
}
