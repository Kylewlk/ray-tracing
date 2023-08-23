//
// Created by wlk12 on 2023/8/7.
//

#include "14RayTracingScene.h"
#include "common/Texture.h"

#include "ray_tracing/hittable_list.h"
#include "ray_tracing/sphere.h"


RayTracingScene::RayTracingScene()
    : BaseScene(ID, 0, 0)
{
    this->cam.type = camera::material;
    this->cam.max_depth = 10;
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 800;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 20;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));


    RayTracingScene::renderImage();
}

RayTracingScene::~RayTracingScene()
{
    this->isRendering = false;
    this->running = false;
    if (this->renderResult.valid())
    {
        this->renderResult.get();
    }
}

SceneRef RayTracingScene::create()
{
    struct enable_make_shared : public RayTracingScene
    {
        enable_make_shared() : RayTracingScene() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RayTracingScene::renderImage()
{
    if (this->isRendering)
    {
        this->isRendering = false;
        this->running = false;
    }
    else
    {
        BaseScene::reset();

        this->imageCurrentWidth = this->imageWidth;
        this->imageCurrentHeight = this->imageHeight;

        cam.vfov     = 20;
        cam.lookfrom = point3(13,2,3);
        cam.lookat   = point3(0,0,0);
        cam.vup      = vec3(0,1,0);

        cam.defocus_angle = 0.6;
        cam.focus_dist    = 10.0;

        this->isRendering = true;
        this->running = true;
    }
}

void RayTracingScene::reset()
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    this->samplePerPixel = 20;
    this->fov = 20;

    BaseScene::reset();
}

void RayTracingScene::draw()
{
    if (!this->renderResult.valid())
    {
        if (this->isRendering)
        {
            if (this->sampleCurrentCount < this->samplePerPixel)
            {
                this->renderResult = this->cam.renderAsync(world, this->aspectRatio, imageWidth, renderingPixel, this->running);
            }
            else
            {
                this->isRendering = false;
                this->running = false;
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

void RayTracingScene::drawSpecificProperty()
{
    ImGui::SliderInt("Sampler Count", &samplePerPixel, 1, 1000, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Ray Max Depth", &this->cam.max_depth, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Defocus Angle", &this->defocusAngle, 0, 50, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderFloat("Focus", &this->focusDist, 1, 20, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Fov", &this->fov, 10, 100, "%d", ImGuiSliderFlags_AlwaysClamp);
}
