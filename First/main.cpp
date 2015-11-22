#include "ELAppDelegate.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>


template <typename T>
struct vec2 {
    typedef T       value_type;

    value_type      x;
    value_type      y;

    template <typename U>
    vec2(const U & u) : x(u), y(u) {}

    template <typename U, typename V>
    vec2(const U& u, const V& v) :
        x(u), y(v)    
    {
    }
};

template <typename T>
struct color {
    typedef T       value_type;

    value_type      r;
    value_type      g;
    value_type      b;

    /*color(value_type r1, value_type g1, value_type b1) : 
        r(r1), g(g1), b(b1)
    {
    }*/
};


struct RawColor {
    int     r;
    int     g;
    int     b;
};

struct Compo {
    vec2<float> pos;
    color<int>  color;
};

#define LEN(x) sizeof x / sizeof x[0]

int CALLBACK _tWinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPWSTR		lpCmdLine, 
	int			nShowCmd) 
{
    using elloop::AppDelegate;


    typedef vec2<float> float2;

    vec2<float> f = {10};

    vec2<float> ary[2] = { {10, 11}, {1,0}};
    //vec2<float> ary2[] = { 10, 11 };

    float2 fs1[] = {
        1, 2,
        3, 4,
        5, 6,
        7, 8
    };
    size_t len1 = LEN(fs1);
    for (size_t i=0; i<len1; ++i) {
        float2 temp = fs1[i];
    }

    float2 fs2[] = {
        {1, 2},
        {3, 4},
        {5, 6},
        {7, 8}
    };
    len1 = LEN(fs2);
    for (size_t i =0; i<len1; ++i) {
        float2 t = fs2[i];
    }

    typedef color<int> colori;
    colori cs1[] = {
        { 1, 2, 3 }, 
        { 4, 5, 6 }, 
        { 7, 8, 9 }
    };
    len1 = LEN(cs1);
    for (size_t i=0; i<len1; ++i) {
        colori temp = cs1[i];
    }

    colori cs2[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    len1 = LEN(cs2);
    for (size_t i=0; i<len1; ++i) {
        colori t = cs2[i];
    }


    RawColor rcs1[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    len1 = LEN(rcs1);
    for (size_t i=0; i<len1; ++i) {
        RawColor t = rcs1[i];
    }


    Compo compos[] = {
        {1, 2}, {1, 1, 1},
        {11, 22}, {11, 12, 13}
    };



    return 0;


    AppDelegate app;
    int ret = elloop::Application::getInstance()->run();
    return ret;
}