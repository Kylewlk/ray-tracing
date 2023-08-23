//
// Created by wlk12 on 2023/8/13.
//

#pragma once

#include <future>
#include "hittable.h"
#include "material.h"

class camera {
public:
    enum ShaderType : uint8_t
    {
        none,
        diffuse,
        diffuse_gamma,
        material
    };

    /* Public Camera Parameters Here */
//    double aspect_ratio = 1.0;  // Ratio of image width over height
//    int    image_width  = 100;  // Rendered image width in pixel count

    ShaderType type { none };
    int max_depth         = 10;   // Maximum number of ray bounces into scene
    double vfov = 90;  // Vertical view angle (field of view)
    point3 lookfrom = point3(0,0,0);  // Point camera is looking from
    point3 lookat   = point3(0,0,-1);   // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction
    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus


    void render(const hittable& world, double aspectRatio, int imageWidth, std::vector<uint8_t>& pixelData) {
        initialize(aspectRatio, imageWidth, 1);
        pixelData.clear();
        pixelData.resize(image_width * image_height * 3);

        std::cout << "Image Size: " << image_width << ' ' << image_height << std::endl;

        for (int j = 0; j < image_height; ++j) {
            //            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, world);
                writeColor(pixelData.data(), imageWidth, i, j, pixel_color);
            }
        }
    }

    void render(const hittable& world, double aspectRatio, int imageWidth, int samplesPerPixel, std::vector<uint8_t>& pixelData) {
        initialize(aspectRatio, imageWidth, samplesPerPixel);
        pixelData.clear();
        pixelData.resize(image_width * image_height * 3);

        std::cout << "Image Size: " << image_width << ' ' << image_height << std::endl;

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    if (this->type == none)
                    {
                        pixel_color += ray_color(r, world);
                    }
                    else if (this->type == diffuse || this->type == diffuse_gamma)
                    {
                        pixel_color += ray_color_diffuse(r, this->max_depth, world);
                    }
                    else if (this->type == material)
                    {
                        pixel_color += ray_color_material(r, this->max_depth, world);
                    }
                }
                if (this->type == none || this->type == diffuse)
                {
                    writeColor(pixelData.data(), imageWidth, i, j, samples_per_pixel, pixel_color);
                }
                else
                {
                    writeColorGamma(pixelData.data(), imageWidth, i, j, samples_per_pixel, pixel_color);

                }
            }
        }
    }

    std::future<bool> renderAsync(const hittable& world, double aspectRatio, int imageWidth, std::vector<color>& pixelData) {
        initialize(aspectRatio, imageWidth, 1);
        pixelData.clear();
        pixelData.resize(image_width * image_height);
        auto data = pixelData.data();

        auto renderOneLine = [this, data, &world](int j){
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);

                ray r = get_ray(i, j);
                if (this->type == none)
                {
                    pixel_color += ray_color(r, world);
                }
                else if (this->type == diffuse || this->type == diffuse_gamma)
                {
                    pixel_color += ray_color_diffuse(r, this->max_depth, world);
                }
                else if (this->type == material)
                {
                    pixel_color += ray_color_material(r, this->max_depth, world);
                }

                data[image_width * j + i] = pixel_color;
            }
        };
        auto result = std::async(std::launch::async, [this, renderOneLine](){
            for (int j = 0; j < image_height; ++j) {
                renderOneLine(j);
            }
            return true;
        });
        return result;
    }

private:
    /* Private Camera Variables Here */
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width  = 100;  // Rendered image width in pixel count
    int    samples_per_pixel = 10;   // Count of random samples for each pixel

    int    image_height{};   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3   u, v, w;        // Camera frame basis vectors
    vec3   defocus_disk_u;  // Defocus disk horizontal radius
    vec3   defocus_disk_v;  // Defocus disk vertical radius

    void initialize(double aspectRatio, int imageWidth, int samplesPerPixel) {
        this->aspect_ratio = aspectRatio;
        this->image_width = imageWidth;
        this->samples_per_pixel = samplesPerPixel;

        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    [[nodiscard]] static color ray_color(const ray& r, const hittable& world) {
        hit_record rec;

        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }


    [[nodiscard]] static color ray_color_diffuse(const ray& r, int depth, const hittable& world) {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return {0,0,0};

        if (world.hit(r, interval(0.001, infinity), rec)) {
//            vec3 direction = random_on_hemisphere(rec.normal);
            vec3 direction = rec.normal + random_unit_vector();
            return 0.5 * ray_color_diffuse(ray(rec.p, direction), depth - 1, world);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }

    [[nodiscard]] static color ray_color_material(const ray& r, int depth, const hittable& world) {
        hit_record rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return {0,0,0};

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color_material(scattered, depth-1, world);
            return {0,0,0};
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }


    [[nodiscard]] ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location i,j.

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return {ray_origin, ray_direction};
    }

    [[nodiscard]] point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    [[nodiscard]] vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
