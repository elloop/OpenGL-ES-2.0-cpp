#include "scenes/MipMapTexture.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);
NS_BEGIN(mip_map);

void MipMapTexture::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void MipMapTexture::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool MipMapTexture::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if ( _valid )
    {
        _position       = glGetAttribLocation(_programId, "_position");
        _uv             = glGetAttribLocation(_programId, "_uv");
        _mipmapTexture      = glGetUniformLocation(_programId, "_textureBg");
        _mvp            = glGetUniformLocation(_programId, "_mvp");
    }

    std::vector<std::string> fileNames =
    {
        "images/p32x32.bmp",
        "images/p16x16.bmp",
        "images/p8x8.bmp",
        "images/p4x4.bmp",
        "images/p2x2.bmp",
        "images/p1x1.bmp"
    };

    _mipmapTextureId = loadMipMap(fileNames);

    using CELL::float3;
    _camera._eye = float3(1, 1, 1);
    _camera._look = float3(0.5f, -0.4f, -5.5f);
    _camera._up = float3(0.0f, 1.0f, 0.0f);
    _camera._right = float3(1.0f, 0.0f, 0.0f);

    return _valid;
}

MipMapTexture* MipMapTexture::create()
{
    auto * self = new MipMapTexture();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int MipMapTexture::loadMipMap(const std::vector<std::string> &fileNames)
{
    unsigned int textureId = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    size_t nums = fileNames.size();
    for (size_t i=0; i<nums; ++i) 
    {
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileNames[i].c_str(), 0);

        FIBITMAP *bitmap = FreeImage_Load(format, fileNames[i].c_str(), 0);

        bitmap = FreeImage_ConvertTo24Bits(bitmap);

        BYTE *pixels = FreeImage_GetBits(bitmap);

        int width = FreeImage_GetWidth(bitmap);
        int height = FreeImage_GetHeight(bitmap);

        for (size_t j = 0; j < width*height * 3; j += 3)
        {
            BYTE temp = pixels[j];
            pixels[j] = pixels[j + 2];
            pixels[j + 2] = temp;
        }

        glTexImage2D(GL_TEXTURE_2D, i, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        FreeImage_Unload(bitmap);
    }
   
    return textureId;
}

void MipMapTexture::render()
{
    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    _camera.updateCamera(0.016);

    float groundSize = 100;
    float groundPosition = -5;
    float repeat = 100;

    Vertex ground[] = 
    {
        { -groundSize, groundPosition, -groundSize, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
        { groundSize, groundPosition, -groundSize,  repeat, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f },
        { groundSize, groundPosition, groundSize,   repeat, repeat, 1.0f, 1.0f, 1.0f, 1.0f },

        { -groundSize, groundPosition, -groundSize, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
        { groundSize, groundPosition, groundSize,   repeat, repeat, 1.0f, 1.0f, 1.0f, 1.0f },
        { -groundSize, groundPosition, groundSize,  0.0f, repeat,   1.0f, 1.0f, 1.0f, 1.0f },
    };

    begin();

    matrix4 matWorld(1);
    matrix4 matView = lookAt(_camera._eye, _camera._look, _camera._up);
    matrix4 matProj = perspective(45.0f, width / height, 0.1f, 100.f);
    matrix4 mvp = matProj * matView * matWorld;

    glUniform1i(_mipmapTexture, 0);
    glBindTexture(GL_TEXTURE_2D, _mipmapTextureId);

    glUniformMatrix4fv(_mvp, 1, false, mvp.data());
    
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &ground[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &ground[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(ground) / sizeof (ground[0]));

    end();
}



NS_END(mip_map);
NS_END(elloop);