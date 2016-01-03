#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"

NS_BEGIN(elloop);


class TextureRectangle1 : public ShaderProgram
{
public:
    static TextureRectangle1*      create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    attribute                   position_;
    uniform                     mvp_;
    attribute                   color_;
protected:
    bool                        init();
    TextureRectangle1()
    {
        vsFileName_ = "shaders/color_rectange_vs.glsl";
        fsFileName_ = "shaders/color_rectangle_fs.glsl";
    }
    ~TextureRectangle1()
    {}
};



NS_END(elloop);