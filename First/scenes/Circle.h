#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"

NS_BEGIN(elloop);


class Circle : public ShaderProgram
{
public:
    static Circle*      create();
    void                begin()     override;
    void                end()       override;
    void                render()    override;

    attribute           _position;
    uniform             _mvp;
    attribute           _color;
protected:

    struct Vertex
    {
        CELL::float2      pos;
        CELL::Rgba4Byte   color;
    };

    bool                init();
    Circle()
    {
        _vsFileName = "shaders/color_rectangle_vs.glsl";
        _fsFileName = "shaders/color_rectangle_fs.glsl";
    }
    ~Circle()
    {}
};



NS_END(elloop);