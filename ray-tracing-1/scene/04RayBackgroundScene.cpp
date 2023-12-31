//
// Created by wlk12 on 2023/8/7.
//

#include "04RayBackgroundScene.h"
#include "common/Texture.h"
#include "camera/Camera2D.h"

#include "ray_tracing/vec3.h"
#include "ray_tracing/ray.h"

RayBackgroundScene::RayBackgroundScene()
    : BaseScene(ID, 0, 0)
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    RayBackgroundScene::renderImage();
}

SceneRef RayBackgroundScene::create()
{
    struct enable_make_shared : public RayBackgroundScene
    {
        enable_make_shared() : RayBackgroundScene() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RayBackgroundScene::renderImage()
{
    this->imageCurrentWidth = this->imageWidth;
    this->imageCurrentHeight = this->imageHeight;

    this->imagePixels.clear();
    this->imagePixels.resize(imageWidth * imageHeight * 3);


    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(imageWidth)/double(imageHeight));
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / double(imageWidth);
    auto pixel_delta_v = viewport_v / double(imageHeight);


    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                               - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    for (int j = 0; j < imageHeight; ++j) {
        for (int i = 0; i < imageWidth; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = rayColor(r);
            writeColor(imagePixels.data(), imageWidth, i, j, pixel_color);
        }
    }


    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RayBackgroundScene::reset()
{
    this->imageWidth = 400;
    this->imageHeight = 400;
    this->camera->resetView();
}

color RayBackgroundScene::rayColor(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}
