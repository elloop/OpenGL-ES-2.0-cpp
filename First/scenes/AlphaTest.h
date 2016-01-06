#pragma  once

#include "gl_include.h"
#include "ELShaderProgram.h"
#include <string>
#include <vector>

NS_BEGIN(elloop);
NS_BEGIN(alpha);

class FirstCamera
{
public:
    FirstCamera()
        : _moveSpeed(5)
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


class AlphaTest : public ShaderProgram
{
public:
    static AlphaTest*   create();
    void                        begin()     override;
    void                        end()       override;
    void                        render()    override;

    uniform                     _mvp;
    uniform                     _textureBg;
    attribute                   _position;
    attribute                   _uv;

    unsigned int                _textureBgId;
    unsigned int                _textureDynamic;
    unsigned int                _textureDog;

    FirstCamera                 _camera;

protected:

    struct Vertex
    {
        float x, y, z;
        float u, v;
        float r, g, b, a;
    };

    bool                        init();
    AlphaTest()
        : _mvp(-1)
        , _textureBg(-1)
        , _textureBgId(0)
        , _position(-1)
        , _uv(-1)
        , _textureDynamic(0)
        , _textureDog(0)
    {
        _vsFileName = "shaders/alpha_test_vs.glsl";
        _fsFileName = "shaders/alpha_test_fs.glsl";
    }
    ~AlphaTest()
    {
        glDeleteTextures(1, &_textureBgId);
        glDeleteTextures(1, &_textureDynamic);
        glDeleteTextures(1, &_textureDog);
    }
    unsigned int loadMipMap(const std::vector<std::string> &fileNames);
    unsigned int loadTexture(const std::string &fileName );
    unsigned int createTexture(int width, int height);
};


NS_END(alpha);
NS_END(elloop);