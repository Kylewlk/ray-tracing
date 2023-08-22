//
// Created by wlk12 on 2023/8/7.
//

#include "10RayMetalScene.h"
#include "common/Texture.h"

#include "ray_tracing/hittable_list.h"
#include "ray_tracing/sphere.h"


RayMetalScene::RayMetalScene()
    : BaseScene(ID, 0, 0)
{
    this->cam.type = camera::material;
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 10;
    RayMetalScene::renderImage();
}

SceneRef RayMetalScene::create()
{
    struct enable_make_shared : public RayMetalScene
    {
        enable_make_shared() : RayMetalScene() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RayMetalScene::renderImage()
{
    this->imageCurrentWidth = this->imageWidth;
    this->imageCurrentHeight = this->imageHeight;

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    std::shared_ptr<metal> material_left;
    std::shared_ptr<metal> material_right;
    if (this->fuzzyReflection)
    {
        material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
        material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    }
    else
    {
        material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.0);
        material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    }

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    this->cam.render(world, this->aspectRatio, imageWidth, samplePerPixel, imagePixels);

    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RayMetalScene::reset()
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 10;
    BaseScene::reset();
}

void RayMetalScene::drawSpecificProperty()
{
    ImGui::SliderInt("Sampler Count", &samplePerPixel, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Ray Max Depth", &this->cam.max_depth, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::Checkbox("Fuzzy Reflection", &this->fuzzyReflection);
}
