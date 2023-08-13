//
// Created by wlk12 on 2023/8/13.
//
#pragma once

using color = vec3;    // RGB color

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


inline void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}