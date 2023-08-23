//
// Created by wlk12 on 2023/8/6.
//
#pragma once

#include "scene/Scene.hpp"
#include <future>
#include "ray_tracing/rtweekend.h"

class BaseScene;
using BaseSceneRef = std::shared_ptr<BaseScene>;

class BaseScene : public Scene
{
public:

    ~BaseScene() override = default;
    BaseScene(const BaseScene&) = delete;
    BaseScene& operator=(BaseScene&) = delete;

    [[nodiscard]] int getImageWidth() const { return this->imageWidth; }
    [[nodiscard]] int getImageHeight() const { return this->imageHeight; }
    void setImageSize(int width, int height);

    [[nodiscard]] const std::vector<uint8_t>& getPixels() const { this->imagePixels; }

    virtual void renderImage() = 0;

protected:
    BaseScene(const char* name, int width, int height);

    void onMouseEvent(const MouseEvent* e) override;

    void draw() override;
    void drawProperty() override;
    virtual void reset();
    virtual void drawSpecificProperty();
    void updateImage();

    std::vector<uint8_t> imagePixels;
    int imageCurrentWidth = 256;
    int imageCurrentHeight = 256;
    int sampleCurrentCount = 0;
    int samplePerPixel = 1;
    int imageWidth = 256;
    int imageHeight = 256;
    double aspectRatio = 1.0;

    std::future<bool> renderResult;
    std::atomic<bool> running{false};
    std::vector<color> pixelCache;
    std::vector<color> renderingPixel;

    TextureRef texture;
    ShaderRef shader;
    Camera2DRef camera;

    math::Vec2 lastMousePos{};
    bool holdLeftButton{false};

    bool lockAspectRatio{true};
    bool isRendering{false};
};

