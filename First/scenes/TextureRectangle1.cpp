#include "scenes/TextureRectangle1.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);


void TextureRectangle1::begin()
{
    glUseProgram(programId_);
    glEnableVertexAttribArray(position_);
    glEnableVertexAttribArray(uv_);
}

void TextureRectangle1::end()
{
    glDisableVertexAttribArray(uv_);
    glDisableVertexAttribArray(position_);
    glUseProgram(0);
}

TextureRectangle1* TextureRectangle1::create()
{
    auto * self = new TextureRectangle1();
    if (self && self->init())
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

bool TextureRectangle1::init()
{
    valid_ = ShaderProgram::initWithFile(vsFileName_, fsFileName_);
    if (valid_)
    {
        position_ = glGetAttribLocation(programId_, "position_");
        uv_ = glGetAttribLocation(programId_, "uv_");
        texture_ = glGetUniformLocation(programId_, "texture_");
        mvp_ = glGetUniformLocation(programId_, "mvp_");
    }

    textureId_ = loadTexture("images/main.jpg");
    unsigned int temp = loadTexture("images/fog.bmp");

    return valid_;
}

unsigned int TextureRectangle1::loadTexture(const std::string &fileName)
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    FreeImage_Unload(bitmap);

    return textureId;
}

void TextureRectangle1::render()
{
    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s.width_;
    float height = s.height_;

    glViewport(0, 0, width, height);

    begin();

    matrix4 screenProj = ortho<float>(0, width, height, 0, -100.0f, 100);

    GLfloat x = 100;
    GLfloat y = 100;
    GLfloat w = 400;
    GLfloat h = 400;

    Vertex ary[] =
    {
        float2{ x, y }, float2(0, 0),
        float2{ x + w, y }, float2(1, 0),
        float2{ x, y + h }, float2(0, 1),
        float2{ x + w, y + h }, float2(1, 1),
    };

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    glUniform1i(texture_, 0);

    glUniformMatrix4fv(mvp_, 1, false, screenProj.data());
    glVertexAttribPointer(position_, 2, GL_FLOAT, false, sizeof(Vertex), ary);
    glVertexAttribPointer(uv_, 2, GL_FLOAT, false, sizeof(Vertex), &(ary[0].uv));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}

NS_END(elloop);