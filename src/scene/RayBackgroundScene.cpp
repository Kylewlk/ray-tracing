//
// Created by wlk12 on 2023/8/7.
//

#include "RayBackgroundScene.h"
#include "common/Texture.h"
#include "ray_tracing/vec3.h"

RayBackgroundScene::RayBackgroundScene()
    : BaseScene(ID, 0, 0)
{
    this->imageWidth = 400;
    this->imageHeight = 400;
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

    vec3 color{ 0.0, 0.0, 0.25};
    for (int y = 0; y < imageHeight; ++y)
    {
        color[1] = double(y) / double(imageHeight - 1);

        for (int x = 0; x < imageWidth; ++x)
        {
            color[0] = double(x) / double(imageWidth - 1);

            writeColor(this->imagePixels.data(), imageWidth, x, y, color);
        }
    }
    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, this->imagePixels.data());
}

void RayBackgroundScene::reset()
{
    this->imageWidth = 400;
    this->imageHeight = 400;
    BaseScene::reset();
}