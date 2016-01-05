#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>

NS_BEGIN(elloop);


class SimpleRectangle : public ShaderProgram
{
public:
    static SimpleRectangle*     create();


    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    attribute                   _position;
    uniform                     _mvp;
    uniform                     _color;
protected:
    bool                        init();
    SimpleRectangle() 
    {
        _vsFileName = "shaders/simple_rectange_vs.glsl";
        _fsFileName = "shaders/simple_rectangle_fs.glsl";
    }
    ~SimpleRectangle()
    {}
};



NS_END(elloop);