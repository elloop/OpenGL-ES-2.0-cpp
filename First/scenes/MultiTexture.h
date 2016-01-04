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

    uniform                     mvp_;
    uniform                     textureBg_;
    uniform                     textureCloud_;
    uniform                     uvAnim_;
    attribute                   position_;
    attribute                   uv_;

    unsigned int                textureBgId_;
    unsigned int                textureCloudId_;
protected:
    struct Vertex
    {
        CELL::float2      pos;
        CELL::float2      uv;
    };
    bool                        init();
    MultiTexture()
        : mvp_(-1)
        , textureBg_(-1)
        , textureCloud_(-1)
        , textureBgId_(-1)
        , textureCloudId_(-1)
        , position_(-1)
        , uv_(-1)
        , uvAnim_(-1)
    {
        vsFileName_ = "shaders/multi_texture_vs.glsl";
        fsFileName_ = "shaders/multi_texture_fs.glsl";
    }
    ~MultiTexture()
    {}
    unsigned int loadTexture(const std::string &fileName);
};



NS_END(elloop);