//
// Created by wlk12 on 2023/8/13.
//
#pragma once

using color = vec3;    // RGB color

inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

inline void writeColor(uint8_t* data, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    data[0] = static_cast<uint8_t>(255.999 * pixel_color.x());
    data[1] = static_cast<uint8_t>(255.999 * pixel_color.y());
    data[2] = static_cast<uint8_t>(255.999 * pixel_color.z());
}


inline void writeColor(uint8_t* data, int width, int x, int y, color pixel_color) {

    auto i = (width * y + x) * 3;

    // Write the translated [0,255] value of each color component.
    data[i + 0] = static_cast<uint8_t>(255.999 * pixel_color.x());
    data[i + 1] = static_cast<uint8_t>(255.999 * pixel_color.y());
    data[i + 2] = static_cast<uint8_t>(255.999 * pixel_color.z());
}


inline void writeColor(uint8_t* data, int width, int x, int y, int samples_per_pixel, color pixel_color) {
    static const interval intensity(0.000, 0.999);

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    pixel_color *= scale;

    auto i = (width * y + x) * 3;

    // Write the translated [0,255] value of each color component.
    data[i + 0] = static_cast<uint8_t>(255.999 * intensity.clamp(pixel_color.x()));
    data[i + 1] = static_cast<uint8_t>(255.999 * intensity.clamp(pixel_color.y()));
    data[i + 2] = static_cast<uint8_t>(255.999 * intensity.clamp(pixel_color.z()));
}

inline void writeColorGamma(uint8_t* data, int width, int x, int y, int samples_per_pixel, color pixel_color) {
    static const interval intensity(0.000, 0.999);

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    pixel_color *= scale;

    auto i = (width * y + x) * 3;

    // Write the translated [0,255] value of each color component.
    data[i + 0] = static_cast<uint8_t>(255.999 * intensity.clamp(linear_to_gamma(pixel_color.x())));
    data[i + 1] = static_cast<uint8_t>(255.999 * intensity.clamp(linear_to_gamma(pixel_color.y())));
    data[i + 2] = static_cast<uint8_t>(255.999 * intensity.clamp(linear_to_gamma(pixel_color.z())));
}
