//
// Created by wlk12 on 2023/8/6.
//

#include "Scene.hpp"
#include "common/FrameBuffer.h"

Scene::Scene(const char* name, int width, int height) :name(name), width(width), height(height)
{
    if (width > 0 && height > 0)
    {
        this->fbResolved = FrameBuffer::create(width, height, RenderTarget::kTexColor, RenderTarget::kNone);
        this->fbDraw = FrameBuffer::createMultisample(width, height, 4, true, true);
    }
}

TextureRef Scene::getColorTexture()
{
    return this->fbResolved->getColor();
}


void Scene::resize(int width_, int height_)
{
    if (this->width == width_ && this->height == height_)
    {
        return;
    }

    this->width = width_;
    this->height = height_;
    this->fbResolved = FrameBuffer::create(width, height, RenderTarget::kTexColor, RenderTarget::kNone);
    this->fbDraw = FrameBuffer::createMultisample(width, height, 4, true, true);
}

void Scene::draw()
{

}

void Scene::drawProperty()
{

}

void Scene::render()
{
    fbDraw->bind();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    this->draw();

    fbDraw->unbind();

    fbDraw->blitFramebuffer(0, 0, width, height, this->fbResolved);

    this->drawProperty();
}

