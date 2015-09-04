#include <windows.h>
#include "elloop/inc.h"
#include "elloop/Singleton.h"

NS_BEGIN(elloop);

class Director;

class Application : public Singleton<Application> 
{
public:
    bool                init();
    int                 run();
protected:
    friend Singleton<Application>;
    Application();
private:
    Director*   _director;
};

NS_END(elloop);

