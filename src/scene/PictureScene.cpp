//
// Created by wlk12 on 2023/8/6.
//

#include "PictureScene.hpp"

PictureScene::PictureScene(int width, int height)
    : Scene(ID, width, height)
{

}

SceneRef PictureScene::create()
{
    struct enable_make_shared : public PictureScene
    {
        enable_make_shared() : PictureScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

PictureSceneRef PictureScene::create(int width, int height)
{
    struct enable_make_shared : public PictureScene
    {
        enable_make_shared(int width, int height)
            : PictureScene(width, height) {}
    };

    return std::make_shared<enable_make_shared>(width, height);
}

void PictureScene::draw()
{

}


void PictureScene::drawProperty()
{

}
