#include "scenes/TextureOperations.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);

void TextureOperations::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void TextureOperations::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool TextureOperations::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if ( _valid )
    {
        _position       = glGetAttribLocation(_programId, "_position");
        _uv             = glGetAttribLocation(_programId, "_uv");
        _texture        = glGetUniformLocation(_programId, "_texture");
        _mvp            = glGetUniformLocation(_programId, "_mvp");
    }

    _textureId = loadTexture("images/dog.png");

    return _valid;
}

TextureOperations* TextureOperations::create()
{
    auto * self = new TextureOperations();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int TextureOperations::loadTexture(const std::string &fileName)
{
    unsigned int textureId = 0;

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);
    FIBITMAP *bitmap = FreeImage_Load(format, fileName.c_str(), 0);
    bitmap = FreeImage_ConvertTo24Bits(bitmap);

    BYTE *pixels = FreeImage_GetBits(bitmap);

    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);

    for (size_t i = 0; i < width*height * 3; i += 3)
    {
        BYTE temp = pixels[i];
        pixels[i] = pixels[i + 2];
        pixels[i + 2] = temp;
    }

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // default wrap method, repeat.
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

    // horizontal clamp.
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

    // vertical clamp.
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

     // both clamp.
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

    // mirrored repeat.
   /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);*/

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // draw a white block of size(100, 100) in the texture offset(50, 50).
    char white[100 * 100 * 3];
    memset(white, 255, sizeof(white));
    glTexSubImage2D(GL_TEXTURE_2D, 0, 50, 50, 100, 100, GL_RGB, GL_UNSIGNED_BYTE, white);

    FreeImage_Unload(bitmap);

    return textureId;
}

void TextureOperations::render()
{
    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    float x = 100;
    float y = 100;
    float w = 400;
    float h = 400;

    float repeat = 2;


    Vertex ary[] =
    {
        float2(x, y),           float2(0, 0),          
        float2(x + w, y),       float2(repeat, 0),     
        float2(x, y + h),       float2(0, repeat),     
        float2(x + w, y + h),   float2(repeat, repeat),
    };

    begin();

    matrix4 matProj = ortho<float>(0, width, height,0, -100, 100);

    glUniform1i(_texture, 0);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glUniformMatrix4fv(_mvp, 1, false, matProj.data());
    
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, sizeof(Vertex), &ary[0].pos);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &ary[0].uv);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(ary) / sizeof (ary[0]));

    end();
}



NS_END(elloop);