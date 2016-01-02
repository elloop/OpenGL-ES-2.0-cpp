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

    attribute                   position_;
    uniform                     mvp_;
    uniform                     color_;
protected:
    bool                        init();
    SimpleRectangle() 
    {
        vsFileName_ = "shaders/simple_rectange_vs.glsl";
        fsFileName_ = "shaders/simple_rectangle_fs.glsl";
    }
    ~SimpleRectangle()
    {}
};



NS_END(elloop);