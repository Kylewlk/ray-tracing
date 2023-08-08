//
// Created by wlk12 on 2023/8/6.
//
#pragma once

#include "Scene.hpp"

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

    std::vector<uint8_t> imagePixels;
    int imageCurrentWidth = 256;
    int imageCurrentHeight = 256;
    int imageWidth = 256;
    int imageHeight = 256;
    double aspectRatio = 1.0;
    double viewportHeight = 2.0;
    double viewportWidth = 2.0;

    TextureRef texture;
    ShaderRef shader;
    Camera2DRef camera;

    math::Vec2 lastMousePos{};
    bool holdLeftButton{false};

    bool lockAspectRatio{true};
};

