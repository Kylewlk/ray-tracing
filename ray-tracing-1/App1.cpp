//
// Created by DELL on 2022/3/3.
//

#include "App.h"

#include "scene/PictureScene.hpp"
#include "scene/01CreateImageScene.h"
#include "scene/04RayBackgroundScene.h"
#include "scene/05RaySphereScene.h"
#include "scene/06RaySphereScene2.h"
#include "scene/08RayAntialiasingScene.h"
#include "scene/09RayDiffuseScene.h"
#include "scene/10RayMetalScene.h"
#include "scene/11RayRefractionScene.h"
#include "scene/12RayViewingScene.h"
#include "scene/13RayDefocusBlurScene.h"
#include "scene/14RayTracingScene.h"

#define ADD_SCENE_MENU(scene) {scene::ID, &scene::create}

AppMenu mainMenus[]{
    ADD_SCENE_MENU(PictureScene),
    ADD_SCENE_MENU(CreateImageScene),
    ADD_SCENE_MENU(RayBackgroundScene),
    ADD_SCENE_MENU(RaySphereScene),
    ADD_SCENE_MENU(RaySphereScene2),
    ADD_SCENE_MENU(RayAntialiasingScene),
    ADD_SCENE_MENU(RayDiffuseScene),
    ADD_SCENE_MENU(RayMetalScene),
    ADD_SCENE_MENU(RayRefractionScene),
    ADD_SCENE_MENU(RayViewingScene),
    ADD_SCENE_MENU(RayDefocusBlurScene),
    ADD_SCENE_MENU(RayTracingScene),
};
int mainMenuCount = sizeof(mainMenus)/(sizeof (mainMenus[0]));

