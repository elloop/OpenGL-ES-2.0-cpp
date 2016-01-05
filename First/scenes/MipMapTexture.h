#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>
#include <vector>

NS_BEGIN(elloop);
NS_BEGIN(mip_map);

class ACamera
{
public:
    ACamera()
        : _moveSpeed(20)
        , _eye(0, 10, 0)
        , _look(0.5, -0.4, -0.5)
        , _up(0, 1, 0)
        , _right(1, 0, 0)
    {
    }
    CELL::float3    _eye;
    CELL::float3    _look;
    CELL::float3    _up;
    CELL::float3    _right;
    float           _moveSpeed;

    void updateCamera(float dt)
    {
        using namespace CELL;
        float3 tempLook = _look;
        float3 direction = _look - _eye;
        direction = normalize(direction);
        unsigned char keys[300];
        GetKeyboardState(keys);
        if (keys[VK_UP] & 0x80) 
        {
            _eye  -= direction * (-_moveSpeed) * dt;
            _look -= direction * (-_moveSpeed) * dt;
        }

        if (keys[VK_DOWN] & 0x80)
        {
            _eye += direction * (-_moveSpeed) * dt;
            _look += direction * (-_moveSpeed) * dt;
        }

        if (keys[VK_LEFT] & 0x80)
        {
            _eye -= _right * _moveSpeed * dt;
            _look -= _right * _moveSpeed * dt;
        }

        if (keys[VK_RIGHT] & 0x80)
        {
            _eye += _right * _moveSpeed * dt;
            _look += _right * _moveSpeed * dt;
        }
    }
};


class MipMapTexture : public ShaderProgram
{
public:
    static MipMapTexture*       create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    uniform                     _mvp;
    uniform                     _mipmapTexture;
    attribute                   _position;
    attribute                   _uv;

    unsigned int                _mipmapTextureId;

    ACamera                     _camera;

protected:

    struct Vertex
    {
        float x, y, z;
        float u, v;
        float r, g, b, a;
    };

    bool                        init();
    MipMapTexture()
        : _mvp(-1)
        , _mipmapTexture(-1)
        , _mipmapTextureId(-1)
        , _position(-1)
        , _uv(-1)
    {
        _vsFileName = "shaders/3D_projection_vs.glsl";
        _fsFileName = "shaders/3D_projection_fs.glsl";
    }
    ~MipMapTexture()
    {
        glDeleteTextures(1, &_mipmapTextureId);
    }
    unsigned int loadTexture(const std::string &fileName);
    unsigned int loadMipMap(const std::vector<std::string> &fileNames);
};


NS_END(mip_map);
NS_END(elloop);