#include "ELAppDelegate.h"
#include "ELDirector.h"
#include "include_scenes.h"
#include <ctime>

NS_BEGIN(elloop);


bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    assert(director);

    srand(time(0));

#define TEST_FLAG 12

#if TEST_FLAG == 1          // simple rectangle. pure color.

    auto simpleRectangle = SimpleRectangle::create();
    director->pushScene(simpleRectangle);

#elif TEST_FLAG == 2        // colorful rectangle. 
    
    auto colorRect = ColorRectangle::create();
    director->pushScene(colorRect);

#elif TEST_FLAG == 3        // circle.
    
    auto circle = Circle::create();
    director->pushScene(circle);

#elif TEST_FLAG == 4         // rotate.
   
    auto rotateRectangle = RotateRectangle::create();
    director->pushScene(rotateRectangle);

#elif TEST_FLAG == 5         // texture rectangle 1.
   
    auto texture1 = SimpleTexutre::create();
    director->pushScene(texture1);

#elif TEST_FLAG == 6         // multi texture.

    auto multiTexture = MultiTexture::create();
    director->pushScene(multiTexture);

#elif TEST_FLAG == 7        // simple 3D projection
    
    auto simple3DProjection = Basic3DProjection::create();
    director->pushScene(simple3DProjection);

#elif TEST_FLAG == 8        // mip map
    
    auto mipMapTexture = mip_map::MipMapTexture::create();
    director->pushScene(mipMapTexture);

#elif TEST_FLAG == 9        // texture operations.
    
    auto textureOp = TextureOperations::create();
    director->pushScene(textureOp);

#elif TEST_FLAG == 10       // comprehensive test. include 0 ~ 9 knowledge.

    auto comTest = comprehensive_test::ComprehensiveTest::create();
    director->pushScene(comTest);

#elif TEST_FLAG == 11       // alpha test. 

    auto alphaTest = alpha::AlphaTest::create();
    director->pushScene(alphaTest);

#elif TEST_FLAG == 12       // vertex buffer object.

    auto vboScene = vbo_scene::VboScene::create();
    director->pushScene(vboScene);
#endif

    return true;
}

AppDelegate::AppDelegate() : Application()
{

}

AppDelegate::~AppDelegate()
{

}

NS_END(elloop);