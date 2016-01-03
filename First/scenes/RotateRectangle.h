#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"

NS_BEGIN(elloop);


class RotateRectangle : public ShaderProgram
{
public:
    static RotateRectangle*     create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    attribute                   position_;
    uniform                     mvp_;
    attribute                   color_;
protected:
    bool                        init();
    RotateRectangle()
    {
        vsFileName_ = "shaders/color_rectange_vs.glsl";
        fsFileName_ = "shaders/color_rectangle_fs.glsl";
    }
    ~RotateRectangle()
    {}
};



NS_END(elloop);