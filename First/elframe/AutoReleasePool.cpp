#include "AutoReleasePool.h"
#include "Ref.h"

NS_BEGIN(elloop);

PoolManager* PoolManager::_instance = nullptr;

void AutoReleasePool::purge() {
    std::for_each(_objs.begin(), _objs.end(), [](Ref* obj) {
        if (obj) {
            obj->release();
        }
    });
    _objs.clear();
    ObjectList().swap(_objs);
}

NS_END(elloop);