#pragma  once

#include <vector>
#include <algorithm>
#include <stack>
#include "elloop/inc.h"

NS_BEGIN(elloop);

class Ref;
class PoolManager;

class AutoReleasePool
{
    typedef std::vector<Ref*> ObjectList;
    void purge();

    friend class Ref;
    friend class PoolManager;

    void push(Ref* ref)
    {
        _objs.push_back(ref);
    }

    AutoReleasePool()
    {
        _objs.clear();
    }
    ~AutoReleasePool()
    {
        purge();
    }
    ObjectList  _objs;
};

class PoolManager
{
    typedef std::stack<AutoReleasePool*> PoolStack;

    static PoolManager*                 _instance;
    PoolStack                           _pools;


    PoolManager() {}
    ~PoolManager() {}

public:
    void purge()
    {
        while (!_pools.empty())
        {
            auto pool = _pools.top();
            delete pool;
            _pools.pop();
        }
    }

    static PoolManager* getInstance()
    {
        if (!_instance)
        {
            _instance = new PoolManager();
        }
        return _instance;
    }

    void push()
    {
        auto pool = new AutoReleasePool();
        _pools.push(pool);
    }
    void pop()
    {
        if (!_pools.empty())
        {
            delete _pools.top();
            _pools.pop();
        }
    }
    void autorelease(Ref* ref)
    {
        if (_pools.empty())
        {
            push();
        }
        auto pool = _pools.top();
        pool->push(ref);
    }

    void recycle()
    {
        if (!_pools.empty())
        {
            auto pool = _pools.top();
            pool->purge();
        }
    }
};

NS_END(elloop);