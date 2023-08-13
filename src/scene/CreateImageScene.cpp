//
// Created by wlk12 on 2023/8/6.
//

#include "CreateImageScene.hpp"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera2D.h"
#include "common/Logger.h"
#include "ray_tracing/rtweekend.h"

CreateImageScene::CreateImageScene(int width, int height)
    : Scene(ID, width, height)
{
    this->texture = Texture::create(GL_RGB8, imageWidth, imageHeight);
    this->shader = Shader::createByPath("asset/shader/v_mvp_pos_tex.vert", "asset/shader/f_draw_tex.frag");
    this->camera = Camera2D::create();

    this->renderImage();
}

SceneRef CreateImageScene::create()
{
    struct enable_make_shared : public CreateImageScene
    {
        enable_make_shared() : CreateImageScene(0, 0) {}
    };
    return std::make_shared<enable_make_shared>();
}

CreateImageSceneRef CreateImageScene::create(int width, int height)
{
    struct enable_make_shared : public CreateImageScene
    {
        enable_make_shared(int width, int height)
            : CreateImageScene(width, height) {}
    };

    return std::make_shared<enable_make_shared>(width, height);
}

void CreateImageScene::renderImage()
{
    vec3 color{ 0.0, 0.0, 0.25};
    for (int y = 0; y < imageHeight; ++y)
    {
        color[1] = double(y) / double(imageHeight - 1);

        for (int x = 0; x < imageWidth; ++x)
        {
            color[0] = double(x) / double(imageWidth - 1);

            writeColor(this->pixels, imageWidth, x, y, color);
        }
    }
    this->texture->update(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

void CreateImageScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    auto mat = camera->getViewProj();
    mat = mat * math::scale({(float)texture->getWidth() * 0.5f, (float)texture->getHeight() * 0.5f, 1.0f});

    shader->use();
    glUniformMatrix4fv(1, 1, false, (float*)&mat);
    glUniform4f(2, 1, 1, 1, 1);
    shader->bindTexture(3, this->texture);
    drawQuad();
}


void CreateImageScene::drawProperty()
{
    if (!showPropertyWindow)
    {
        return;
    }

    if(ImGui::Begin(Scene::PropertyWindow, &showPropertyWindow, 0))
    {
        ImGui::SetWindowSize({300, 400}, ImGuiCond_FirstUseEver);
        if (ImGui::Button("Reset", {100.0f, 0}))
        {
            this->camera->resetView();
        }

        ImGui::Separator();

        if (ImGui::Button("Save", {100.0f, 0}))
        {
            constexpr const char* path = ".data/create-image.png";
//            stbi_flip_vertically_on_write(true);
            stbi_write_png(path, imageWidth, imageHeight, 3, pixels, imageWidth * 3);

            auto workingDir = std::filesystem::current_path().u8string();
            LOGI("Save to picture: {}/{}", (const char*)workingDir.data(), path);
        }

    }
    ImGui::End();
}

void CreateImageScene::onMouseEvent(const MouseEvent* e)
{
    if (e->mouseButton == MouseEvent::kButtonLeft)
    {
        if (e->mouseEventType == MouseEvent::kMousePress)
        {
            this->holdLeftButton = true;
        }
        else if (e->mouseEventType == MouseEvent::kMouseRelease)
        {
            this->holdLeftButton = false;
        }
    }

    if (e->mouseEventType == MouseEvent::kMouseScroll)
    {
        float scale = this->camera->getViewScale();
        scale = (e->scrollY > 0) ? scale * 0.8f : scale * 1.25f;
        this->camera->setViewScale(scale);
    }
    else if (e->mouseEventType == MouseEvent::kMouseMove)
    {
        if (this->holdLeftButton)
        {
            auto delta = e->posDelta;
            delta *= this->camera->getViewScale();
            this->camera->move({delta.x, delta.y, 0});
        }
    }
}