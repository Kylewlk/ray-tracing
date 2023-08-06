//
// Created by DELL on 2022/3/3.
//
#pragma once
#include "Define.h"
//#include "scene/BaseScene.h"
#include "common/EventSystem.h"
#include "common/MathHelp.h"
#include "GLWindow.h"

class App
{
public:

    static App* get();

    [[nodiscard]] const ImFont* getFont() const { return this->defaultFont; }

    virtual void run();
    static void resetOpenGLStates();

    [[nodiscard]] math::Vec2 getMainScenePos() const { return {mainSceneX, mainSceneY}; }

    template<class T>
    void mainScene2Screen(T& p) const
    {
        p.x += static_cast<float>(mainSceneX);
        p.y += static_cast<float>(mainSceneY);
    }

    [[nodiscard]] ImFont* getDefaultFont() const { return this->defaultFont; }

protected:
    virtual void init();
    virtual void clean();
    virtual void render();
    virtual void showDockSpace();
    virtual void setupMouseEvent();

    int winWidth = 1600;
    int winHeight = 750;

    ImFont* defaultFont{};

    CustomEventListenerRef sceneCreateEvent;

    GLFWmousebuttonfun oldMouseButtonFun{nullptr};
    GLFWcursorposfun oldCursorPosFun{nullptr};
    GLFWscrollfun oldScrollFun{nullptr};
    bool mainSceneActive{false};
    bool mainSceneHovered{false};
    int mainSceneX{0}, mainSceneY{0};
    std::vector<bool> holdMouseButtons;
};
