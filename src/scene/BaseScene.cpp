//
// Created by wlk12 on 2023/8/7.
//

#include "BaseScene.h"
#include "common/Texture.h"
#include "common/Shader.h"
#include "common/FrameBuffer.h"
#include "common/EventSystem.h"
#include "camera/Camera2D.h"
#include "common/Logger.h"
#include "ray_tracing/vec3.h"

BaseScene::BaseScene(const char* name, int width, int height)
    : Scene(name, width, height)
{
    this->shader = Shader::createByPath("asset/shader/v_mvp_pos_tex.vert", "asset/shader/f_draw_tex.frag");
    this->camera = Camera2D::create();
}

void BaseScene::setImageSize(int width, int height)
{
    this->imageWidth = width;
    this->imageHeight = height;
}

void BaseScene::draw()
{
    this->camera->setViewSize((float)this->width, (float)this->height);
    this->camera->update();

    if (this->texture == nullptr)
    {
        return;
    }

    auto mat = camera->getViewProj();
    mat = mat * math::scale({(float)texture->getWidth() * 0.5f, (float)texture->getHeight() * 0.5f, 1.0f});

    shader->use();
    glUniformMatrix4fv(1, 1, false, (float*)&mat);
    glUniform4f(2, 1, 1, 1, 1);
    shader->bindTexture(3, this->texture);
    drawQuad();
}


void BaseScene::drawProperty()
{
    if (!showPropertyWindow)
    {
        return;
    }

    ImGui::Begin(Scene::PropertyWindow, &showPropertyWindow, ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowSize({300, 400}, ImGuiCond_FirstUseEver);

    ImGui::Text("Image Size: %d, %d, sampler: %d", this->imageCurrentWidth, this->imageCurrentHeight, this->samplerPerPixel);
    ImGui::Separator();

    if (ImGui::Button("Reset", {100.0f, 0}))
    {
        this->reset();
    }

    char aspectRatioCheckBox[32] = {0};
    snprintf(aspectRatioCheckBox, sizeof(aspectRatioCheckBox), "Lock Aspect Ratio(%.3f)", this->aspectRatio);
    ImGui::Checkbox(aspectRatioCheckBox, &lockAspectRatio);
    if(ImGui::SliderInt("Image Width", &imageWidth, 4, 4096, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
        if (lockAspectRatio)
        {
            imageHeight = (int)(double(imageWidth)/aspectRatio);
            if (imageHeight < 4 || imageHeight > 4096)
            {
                imageHeight = std::clamp(imageHeight, 4, 4096);
                aspectRatio = double(imageWidth)/double(imageHeight);
            }
        }
        else
        {
            aspectRatio = double(imageWidth)/double(imageHeight);
        }
    }
    if(ImGui::SliderInt("Image Height", &imageHeight, 4, 4096, "%d", ImGuiSliderFlags_AlwaysClamp))
    {
        if (lockAspectRatio)
        {
            imageWidth = (int)(double(imageHeight) * aspectRatio);
            if (imageWidth < 4 || imageWidth > 4096)
            {
                imageWidth = std::clamp(imageWidth, 4, 4096);
                aspectRatio = double(imageWidth)/double(imageHeight);
            }
        }
        else
        {
            aspectRatio = double(imageWidth)/double(imageHeight);
        }
    }

    this->drawSpecificProperty();

    ImGui::Separator();

    if (ImGui::Button("render", {100.0f, 0}))
    {
        LOGI("Render, {}", this->name);
        this->renderImage();
    }

    if (ImGui::Button("Save", {100.0f, 0}))
    {
        std::string path = ".data/";
        path += this->name;
        path += ".png";
        //            stbi_flip_vertically_on_write(true);
        stbi_write_png(path.c_str(), imageWidth, imageHeight, 3, this->imagePixels.data(), imageWidth * 3);

        auto workingDir = std::filesystem::current_path().u8string();
        LOGI("Save to picture: {}/{}", (const char*)workingDir.data(), path);
    }

    ImGui::End();
}

void BaseScene::onMouseEvent(const MouseEvent* e)
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

void BaseScene::reset()
{
    this->camera->resetView();
}

void BaseScene::drawSpecificProperty()
{

}

