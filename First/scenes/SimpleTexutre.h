#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>

NS_BEGIN(elloop);


class SimpleTexutre : public ShaderProgram
{
public:
    static SimpleTexutre*   create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    uniform                     _mvp;
    uniform                     _texture;
    attribute                   _position;
    attribute                   _uv;

    unsigned int                _textureId;

protected:

    struct Vertex
    {
        CELL::float2      pos;
        CELL::float2      uv;
    };

    bool                        init();
    SimpleTexutre()
        : _mvp(-1)
        , _texture(-1)
        , _textureId(-1)
        , _position(-1)
        , _uv(-1)
    {
        _vsFileName = "shaders/simple_texture_vs.glsl";
        _fsFileName = "shaders/simple_texture_fs.glsl";
    }
    ~SimpleTexutre()
    {
        glDeleteTextures(1, &_textureId);
    }

    unsigned int loadTexture(const std::string &fileName);
};



NS_END(elloop);