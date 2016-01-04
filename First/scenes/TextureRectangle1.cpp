#include "scenes/TextureRectangle1.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);

using namespace CELL;

struct Vertex
{
    float2      pos;
    float2      uv;
    Rgba4Byte   color;
};

void TextureRectangle1::render()
{
    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    begin();

    matrix4 screenProj = ortho<float>(0, width, height, 0, -100.0f, 100);
    GLfloat x = 100;
    GLfloat y = 100;
    GLfloat w = 100;
    GLfloat h = 100;

    Vertex ary[] =
    {
        float2{ x, y }, float2(0, 0), Rgba4Byte(255, 0, 0, 1),
        float2{ x, y + h }, float2(0, 1), Rgba4Byte(0, 255, 0, 1),
        float2{ x + w, y }, float2(1, 0), Rgba4Byte(0, 0, 255, 1),
        float2{ x + w, y + h }, float2(1, 1), Rgba4Byte(255, 255, 255, 1),
    };
    glBindTexture(GL_SAMPLER_2D, textureId_);
    glUniformMatrix4fv(mvp_, 1, false, screenProj.data());
    glUniform1i(texture_, 0);
    glVertexAttribPointer(position_, 2, GL_FLOAT, false, sizeof( Vertex ), ary);
    glVertexAttribPointer(uv_, 2, GL_FLOAT, false, sizeof( Vertex ), &( ary[0].uv ));
    glVertexAttribPointer(color_, 4, GL_UNSIGNED_BYTE, true, sizeof( Vertex ), &( ary[0].color ));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}

void TextureRectangle1::begin()
{
    glUseProgram(programId_);
    glEnableVertexAttribArray(position_);
    glEnableVertexAttribArray(color_);
    glEnableVertexAttribArray(uv_);
}

void TextureRectangle1::end()
{
    glDisableVertexAttribArray(uv_);
    glDisableVertexAttribArray(color_);
    glDisableVertexAttribArray(position_);
    glUseProgram(0);
}

bool TextureRectangle1::init()
{
    valid_ = ShaderProgram::initWithFile(vsFileName_, fsFileName_);
    if ( valid_ )
    {
        position_ = glGetAttribLocation(programId_, "_position");
        uv_ = glGetAttribLocation(programId_, "_uv");
        color_ = glGetAttribLocation(programId_, "_color");
        texture_ = glGetUniformLocation(programId_, "_textureId");
        mvp_ = glGetUniformLocation(programId_, "_mvp");
    }

    return valid_;
}

TextureRectangle1* TextureRectangle1::create()
{
    auto * self = new TextureRectangle1();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

void TextureRectangle1::loadTexture(const std::string &fileName)
{
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);
    FIBITMAP *bitmap = FreeImage_Load(format, fileName.c_str(), 0);
    bitmap = FreeImage_ConvertTo24Bits(bitmap);
    BYTE *pixels = FreeImage_GetBits(bitmap);
    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    glGenTextures(1, &textureId_);
    glBindTexture(GL_SAMPLER_2D, textureId_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels
        );
    FreeImage_Unload(bitmap);   
}

NS_END(elloop);