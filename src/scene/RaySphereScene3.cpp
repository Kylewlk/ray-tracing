//
// Created by wlk12 on 2023/8/7.
//

#include "RaySphereScene3.h"
#include "common/Texture.h"

#include "ray_tracing/rtweekend.h"
#include "ray_tracing/hittable.h"
#include "ray_tracing/hittable_list.h"
#include "ray_tracing/sphere.h"


RaySphereScene3::RaySphereScene3()
    : BaseScene(ID, 0, 0)
{
    this->aspectRatio = 16.0 / 9.0;
    this->imageWidth = 400;
    this->imageHeight = int(double(imageWidth)/aspectRatio);
    RaySphereScene3::renderImage();
}

SceneRef RaySphereScene3::create()
{
    struct enable_make_shared : public RaySphereScene3
    {
        enable_make_shared() : RaySphereScene3() {}
    };
    return std::make_shared<enable_make_shared>();
}

void RaySphereScene3::renderImage()
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

    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Render
    for (int j = 0; j < imageHeight; ++j) {
        for (int i = 0; i < imageWidth; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = rayColor(r, world);
            writeColor(imagePixels.data(), imageWidth, i, j, pixel_color);
        }
    }


    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RaySphereScene3::reset()
{
    this->imageWidth = 400;
    this->imageHeight = 400;
    BaseScene::reset();
}

color RaySphereScene3::rayColor(const ray& r, const hittable& world)
{
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }


    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}
