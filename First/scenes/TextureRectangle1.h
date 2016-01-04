#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>

NS_BEGIN(elloop);


class TextureRectangle1 : public ShaderProgram
{
public:
    static TextureRectangle1*      create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    uniform                     mvp_;
    uniform                     texture_;
    attribute                   position_;
    attribute                   uv_;
    attribute                   color_;

    unsigned int                textureId_;
protected:
    bool                        init();
    TextureRectangle1()
    {
        vsFileName_ = "shaders/texture_rect_vs.glsl";
        fsFileName_ = "shaders/texture_rect_fs.glsl";
    }
    ~TextureRectangle1()
    {}
    void loadTexture(const std::string &fileName);
};



NS_END(elloop);