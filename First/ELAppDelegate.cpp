#include "ELAppDelegate.h"
#include "ELDirector.h"
#include "SimpleRectangle.h"

NS_BEGIN(elloop);


bool AppDelegate::applicationDidFinishLaunching() {

    auto director = Director::getInstance();
    if (director) {
        auto simpleRectangle = SimpleRectangle::create();
        director->runWithScene(simpleRectangle);
    }


    return true;
}

AppDelegate::AppDelegate() : Application() {

}

AppDelegate::~AppDelegate() {

}

NS_END(elloop);