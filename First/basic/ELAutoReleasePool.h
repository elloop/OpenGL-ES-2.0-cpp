#pragma  once

#include <vector>
#include <algorithm>
#include <stack>
#include "elloop/inc.h"

class ELWindowApp;

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

    static PoolManager*                 instance_;
    PoolStack                           pools_;


    PoolManager() {}
    ~PoolManager() {}

public:
    void purge()
    {
        while (!pools_.empty())
        {
            auto pool = pools_.top();
            delete pool;
            pools_.pop();
        }
    }

    static PoolManager* getInstance()
    {
        if (!instance_)
        {
            instance_ = new PoolManager();
        }
        return instance_;
    }

    void push()
    {
        auto pool = new AutoReleasePool();
        pools_.push(pool);
    }
    void pop()
    {
        if (!pools_.empty())
        {
            delete pools_.top();
            pools_.pop();
        }
    }
    void autorelease(Ref* ref)
    {
        if (pools_.empty())
        {
            push();
        }
        auto pool = pools_.top();
        pool->push(ref);
    }

    void recycle()
    {
        if (!pools_.empty())
        {
            auto pool = pools_.top();
            pool->purge();
        }
    }
};

NS_END(elloop);