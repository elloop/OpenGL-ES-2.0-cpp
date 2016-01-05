#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>
#include <vector>

NS_BEGIN(elloop);

class TextureOperations : public ShaderProgram
{
public:
    static TextureOperations*   create();
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
        CELL::float2        pos;
        CELL::float2        uv;
    };

    bool                        init();
    TextureOperations()
        : _mvp(-1)
        , _texture(-1)
        , _textureId(-1)
        , _position(-1)
        , _uv(-1)
    {
        _vsFileName = "shaders/simple_texture_vs.glsl";
        _fsFileName = "shaders/simple_texture_fs.glsl";
    }
    ~TextureOperations()
    {
        glDeleteTextures(1, &_textureId);
    }
    unsigned int loadTexture(const std::string &fileName);
};


NS_END(elloop);