//
// Created by wlk12 on 2023/8/6.
//
#pragma once

#include "Scene.hpp"

class PictureScene;
using PictureSceneRef = std::shared_ptr<PictureScene>;

class PictureScene : public Scene
{
public:
    static constexpr const char* ID = "Picture Scene";

    static SceneRef create();
    static PictureSceneRef create(int width, int height);

    ~PictureScene() override = default;
    PictureScene(const PictureScene&) = delete;
    PictureScene& operator=(PictureScene&) = delete;


private:
    PictureScene(int width, int height);

    void draw() override;
    void drawProperty() override;

    TextureRef texture;

};

