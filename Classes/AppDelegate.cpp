#include "AppDelegate.h"
#include "HelloWorldScene.h"
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#endif


USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
#ifdef SDKBOX_ENABLED
    sdkbox::PluginAdMob::init();
    sdkbox::PluginAdMob::cache(Ads::FullScreen);
#ifdef COCOS2D_DEBUG
    sdkbox::PluginAdMob::setTestDevices("EDC7F3CA3EDD72B5166A693681DF203F");
#endif
#endif
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("roman2048", Rect(0, 0, 600, 640));
        director->setOpenGLView(glview);
    }
    
    director->getOpenGLView()->setDesignResolutionSize(640, 960, ResolutionPolicy::FIXED_HEIGHT);
    
    // turn on display FPS
    director->setDisplayStats(false);
    
    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("fonts");
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Notifications::enterBackground);
    Director::getInstance()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
