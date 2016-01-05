#include "scenes/Basic3DProjection.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);


void Basic3DProjection::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void Basic3DProjection::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool Basic3DProjection::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if ( _valid )
    {
        _position       = glGetAttribLocation(_programId, "_position");
        _uv             = glGetAttribLocation(_programId, "_uv");
        _textureBg      = glGetUniformLocation(_programId, "_textureBg");
        _mvp            = glGetUniformLocation(_programId, "_mvp");
    }

    _textureBgId = loadTexture("images/1.jpg");

    _camera._eye = CELL::float3(1, 1, 1);
    _camera._look = CELL::float3(0.5f, -0.4f, -5.5f);
    _camera._up = CELL::float3(0.0f, 1.0f, 0.0f);
    _camera._right = CELL::float3(1.0f, 0.0f, 0.0f);

    return _valid;
}

Basic3DProjection* Basic3DProjection::create()
{
    auto * self = new Basic3DProjection();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int Basic3DProjection::loadTexture(const std::string &fileName)
{
    unsigned int textureId = 0;
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);

    FIBITMAP *bitmap = FreeImage_Load(format, fileName.c_str(), 0);

    bitmap = FreeImage_ConvertTo24Bits(bitmap);

    BYTE *pixels = FreeImage_GetBits(bitmap);

    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);

    for (size_t i=0; i<width*height*3; i+=3) 
    {
        BYTE temp = pixels[i];
        pixels[i] = pixels[i+2];
        pixels[i+2] = temp;
    }

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
    FreeImage_Unload(bitmap);   
    return textureId;
}

void Basic3DProjection::render()
{
    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    _camera.updateCamera(0.016);

    /* Vertex ary[] =
     {
     { -1.0f, -1.0f, 1.0f,   0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
     { 1.0f, -1.0f, 1.0f,    1.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
     { 1.0f, 1.0f, 1.0f,     1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f },

     { -1.0f, -1.0f, 1.0f,   0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
     { 1.0f, 1.0f, 1.0f,     1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f },
     { -1.0f, 1.0f, 1.0f,    0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f },
     };*/


    float gSize = 100;
    float gPos = -5;
    float repeat = 100;

    Vertex ground[] = 
    {
        { -gSize, gPos, -gSize, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
        { gSize, gPos, -gSize,  repeat, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f },
        { gSize, gPos, gSize,   repeat, repeat, 1.0f, 1.0f, 1.0f, 1.0f },

        { -gSize, gPos, -gSize, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f, 1.0f },
        { gSize, gPos, gSize,   repeat, repeat, 1.0f, 1.0f, 1.0f, 1.0f },
        { -gSize, gPos, gSize,  0.0f, repeat,   1.0f, 1.0f, 1.0f, 1.0f },
    };

    begin();

    matrix4 matWorld(1);
    matrix4 matView = lookAt(_camera._eye, _camera._look, _camera._up);
    matrix4 matProj = perspective(45.0f, width / height, 0.1f, 100.f);

    matrix4 mvp = matProj * matView * matWorld;


    glUniform1i(_textureBg, 0);
    glBindTexture(GL_TEXTURE_2D, _textureBgId);

    glUniformMatrix4fv(_mvp, 1, false, mvp.data());
    
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &ground[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &ground[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(ground) / sizeof (ground[0]));

    end();
}
NS_END(elloop);