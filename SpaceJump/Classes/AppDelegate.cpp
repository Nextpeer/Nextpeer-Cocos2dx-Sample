//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "CCNextpeer.h"
using namespace nextpeer;

USING_NS_CC;

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(320, 480),   "iphone" };
static Resource mediumResource =  { cocos2d::CCSizeMake(768, 1024),  "ipad"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(1536, 2048), "ipadhd" };
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(320, 480);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    
    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionExactFit);
    CCSize frameSize = pEGLView->getFrameSize();
    std::vector<std::string> searchPath;
    
    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    
    // if the frame's height is larger than the height of medium resource size, select large resource.
    float scaleFactor = 1.0f;
    if (frameSize.height > mediumResource.size.height) {
        searchPath.push_back(largeResource.directory);
        scaleFactor = 4.0f;
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height) {
        searchPath.push_back(mediumResource.directory);
        scaleFactor = 2.0f;
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else {
        searchPath.push_back(smallResource.directory);
        scaleFactor = 1.0f;
    }
    pDirector->setContentScaleFactor(scaleFactor);
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    // Init Nextpeer
    this->initializeNextpeer();
    
    // run
    pDirector->runWithScene(MainMenuScene::scene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
}

void AppDelegate::initializeNextpeer()
{
    // Use the proper appKey according to the current platform
    // Those keys will work with the current bundle name ("com.nextpeer.cocos2dx.sample")
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const char* appKey = "c35282cca868f462997a36737581f056";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    const char* appKey = "cf1f724ce11a9d7ae14ce7bd9d274e91";
#endif
    CCNextpeer::getInstance()->initialize(appKey);
    
    // Register for events
    _nextpeerDelegate.registerForEvents();
}
