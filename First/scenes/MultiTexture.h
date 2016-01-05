#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>

NS_BEGIN(elloop);


class MultiTexture : public ShaderProgram
{
public:
    static MultiTexture*        create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    uniform                     _mvp;
    uniform                     _textureBg;
    uniform                     _textureCloud;
    uniform                     _deltaUv;
    attribute                   _position;
    attribute                   _uv;

    unsigned int                _textureBgId;
    unsigned int                _textureCloudId;
protected:
    struct Vertex
    {
        CELL::float2      pos;
        CELL::float2      uv;
    };
    bool                        init();
    MultiTexture()
        : _mvp(-1)
        , _textureBg(-1)
        , _textureCloud(-1)
        , _textureBgId(-1)
        , _textureCloudId(-1)
        , _position(-1)
        , _uv(-1)
        , _deltaUv(-1)
    {
        _vsFileName = "shaders/multi_texture_vs.glsl";
        _fsFileName = "shaders/multi_texture_fs.glsl";
    }
    ~MultiTexture()
    {
        glDeleteTextures(1, &_textureBgId);
        glDeleteTextures(1, &_textureCloudId);
    }
    unsigned int loadTexture(const std::string &fileName);
};



NS_END(elloop);