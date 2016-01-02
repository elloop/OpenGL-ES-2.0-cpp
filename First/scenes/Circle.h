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

    attribute           position_;
    uniform             mvp_;
    attribute           color_;
protected:

    struct Vertex
    {
        CELL::float2      pos;
        CELL::Rgba4Byte   color;
    };

    bool                init();
    Circle()
    {
        vsFileName_ = "shaders/circle_vs.glsl";
        fsFileName_ = "shaders/circle_fs.glsl";
    }
    ~Circle()
    {}
};



NS_END(elloop);