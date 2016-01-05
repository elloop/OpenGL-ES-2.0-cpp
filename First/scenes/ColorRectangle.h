#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"

NS_BEGIN(elloop);


class ColorRectangle : public ShaderProgram
{
public:
    static ColorRectangle*      create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    attribute                   _position;
    uniform                     _mvp;
    attribute                   _color;
protected:
    bool                        init();
    ColorRectangle()
    {
        _vsFileName = "shaders/color_rectangle_vs.glsl";
        _fsFileName = "shaders/color_rectangle_fs.glsl";
    }
    ~ColorRectangle()
    {}
};



NS_END(elloop);