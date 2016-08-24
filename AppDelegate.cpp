#include "AppDelegate.h"
#include "PublicDef.h"
#include "ui/HallScene.h"

USING_NS_CC;
USING_NS_MIGU_COCOS;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("MiguWawaCocosHall");
        director->setOpenGLView(glview);
		glview->setFrameSize(1280, 720);
		glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::FIXED_WIDTH);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    auto scene = HallScene::create();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
