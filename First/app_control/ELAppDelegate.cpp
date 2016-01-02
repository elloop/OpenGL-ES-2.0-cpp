#include "ELAppDelegate.h"
#include "ELDirector.h"
#include "scenes/SimpleRectangle.h"
#include "scenes/ColorRectangle.h"

NS_BEGIN(elloop);


bool AppDelegate::applicationDidFinishLaunching()
{

    auto director = Director::getInstance();

    /*if (director)
    {
        auto simpleRectangle = SimpleRectangle::create();
        director->pushScene(simpleRectangle);
    }*/

    if (director)
    {
        auto colorRect = ColorRectangle::create();
        director->pushScene(colorRect);
    }

    return true;
}

AppDelegate::AppDelegate() : Application()
{

}

AppDelegate::~AppDelegate()
{

}

NS_END(elloop);