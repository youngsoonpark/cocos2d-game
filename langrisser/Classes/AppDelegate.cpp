#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


using namespace cocos2d;

AppDelegate::AppDelegate()
:m_pLuaEngine(NULL)
{

}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

    CCScriptEngineManager::sharedScriptEngineManager()->removeScriptEngine();
    CC_SAFE_DELETE(m_pLuaEngine);
}

bool AppDelegate::initInstance()
{
    bool bRet = false;
    do 
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The HelloWorld is designed as HVGA.
        CCEGLView * pMainWnd = new CCEGLView();
        CC_BREAK_IF(! pMainWnd
            || ! pMainWnd->Create(TEXT("langrisser"), 320, 480));
#endif  // CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

        // OpenGLView initialized in testsAppDelegate.mm on ios platform, nothing need to do here.

#endif  // CC_PLATFORM_IOS

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        // Android doesn't need to do anything.

#endif  // CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
        // Initialize OpenGLView instance, that release by CCDirector when application terminate.
        // The HelloWorld is designed as HVGA.
        // Use GetScreenWidth() and GetScreenHeight() get screen width and height.
        CCEGLView * pMainWnd = new CCEGLView(this);
        CC_BREAK_IF(! pMainWnd
            || ! pMainWnd->Create(320, 480));

#if !defined(_TRANZDA_VM_)
        // set the resource zip file
        // on wophone emulator, we copy resources files to Work7/TG3/APP/ folder instead of zip file
        CCFileUtils::setResource("langrisser.zip");
#endif

#endif  // CC_PLATFORM_WOPHONE

        bRet = true;
    } while (0);
    return bRet;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(&CCEGLView::sharedOpenGLView());

    // sets landscape mode
    pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);

    // turn on display FPS
    pDirector->setDisplayFPS(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // register lua engine
    m_pLuaEngine = new LuaEngine; 
    CCScriptEngineManager::sharedScriptEngineManager()->setScriptEngine(m_pLuaEngine);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    unsigned long size;
    char *pFileContent = (char*)CCFileUtils::getFileData("hello.lua", "r", &size);

    if (pFileContent)
    {
        // copy the file contents and add '\0' at the end, or the lua parser can not parse it
        char *pCodes = new char[size + 1];
        pCodes[size] = '\0';
        memcpy(pCodes, pFileContent, size);
        delete[] pFileContent;

        CCScriptEngineManager::sharedScriptEngineManager()->getScriptEngine()->executeString(pCodes);
        delete []pCodes;
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    string path = CCFileUtils::fullPathFromRelativePath("hello.lua");
    CCScriptEngineManager::sharedScriptEngineManager()->getScriptEngine()->executeScriptFile(path.c_str());
#endif 
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}