#include "Application.h"

NS_BEGIN(elloop);


bool Application::init() 
{

    return true;
}

int Application::run() 
{
    MessageBox(nullptr, TEXT("hwllo"), TEXT("title"), 0);
    return 0;
}

Application::Application() 
    : _director(nullptr) 
{

}

NS_END(elloop);

