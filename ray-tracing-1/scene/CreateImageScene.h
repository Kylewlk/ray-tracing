//
// Created by wlk12 on 2023/8/6.
//
#pragma once

#include "scene/Scene.hpp"

class CreateImageScene;
using CreateImageSceneRef = std::shared_ptr<CreateImageScene>;

class CreateImageScene : public Scene
{
public:
    static constexpr const char* ID = "Create Image";

    static SceneRef create();
    static CreateImageSceneRef create(int width, int height);

    ~CreateImageScene() override = default;
    CreateImageScene(const CreateImageScene&) = delete;
    CreateImageScene& operator=(CreateImageScene&) = delete;

    static constexpr int imageWidth = 256;
    static constexpr  int imageHeight = 256;
    [[nodiscard]] const uint8_t* getPixels() const { this->pixels; }

    void renderImage();

private:
    CreateImageScene(int width, int height);

    void onMouseEvent(const MouseEvent* e) override;

    void draw() override;
    void drawProperty() override;

    uint8_t pixels[imageWidth * imageHeight* 3]{};

    TextureRef texture;
    ShaderRef shader;
    Camera2DRef camera;

    math::Vec2 lastMousePos{};
    bool holdLeftButton{false};
};

