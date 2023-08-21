//
// Created by DELL on 2022/3/3.
//

#include "App.h"

#include "scene/PictureScene.hpp"
#include "scene/CreateImageScene.h"
#include "scene/RayBackgroundScene.h"
#include "scene/RaySphereScene.h"
#include "scene/RaySphereScene2.h"
#include "scene/RayAntialiasingScene.h"
#include "scene/RayDiffuseScene.h"
#include "scene/RayMetalScene.h"
#include "scene/RayRefractionScene.h"
#include "scene/RayViewingScene.h"
#include "scene/RayDefocusBlurScene.h"
#include "scene/RayTracingScene.h"

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
    ADD_SCENE_MENU(RayTracingScene),
    ADD_SCENE_MENU(RayDefocusBlurScene),
};
int mainMenuCount = sizeof(mainMenus)/(sizeof (mainMenus[0]));

