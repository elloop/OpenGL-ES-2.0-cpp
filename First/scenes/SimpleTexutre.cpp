#include "scenes/SimpleTexutre.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);


void SimpleTexutre::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void SimpleTexutre::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

SimpleTexutre* SimpleTexutre::create()
{
    auto * self = new SimpleTexutre();
    if (self && self->init())
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

bool SimpleTexutre::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if (_valid)
    {
        _position = glGetAttribLocation(_programId, "_position");
        _uv = glGetAttribLocation(_programId, "_uv");
        _texture = glGetUniformLocation(_programId, "_texture");
        _mvp = glGetUniformLocation(_programId, "_mvp");
    }

    _textureId = loadTexture("images/dog.png");

    return _valid;
}

unsigned int SimpleTexutre::loadTexture(const std::string &fileName)
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

void SimpleTexutre::render()
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
    GLfloat w = 400;
    GLfloat h = 400;

    Vertex aryReverseUv[] =
    {
        float2{ x, y },         float2(0, 1),
        float2{ x + w, y },     float2(1, 1),
        float2{ x, y + h },     float2(0, 0),
        float2{ x + w, y + h }, float2(1, 0),
    };

    Vertex ary[] =
    {
        float2{ x, y },         float2(0, 0),
        float2{ x + w, y },     float2(1, 0),
        float2{ x, y + h },     float2(0, 1),
        float2{ x + w, y + h }, float2(1, 1),
    };


    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glUniform1i(_texture, 0);

    glUniformMatrix4fv(_mvp, 1, false, screenProj.data());
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, sizeof(Vertex), ary);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &(ary[0].uv));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // picture is reverse
    // use aryReverseUv to reverse texture.

    end();
}

NS_END(elloop);