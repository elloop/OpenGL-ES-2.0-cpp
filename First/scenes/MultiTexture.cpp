#include "scenes/MultiTexture.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);


void MultiTexture::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void MultiTexture::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool MultiTexture::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if ( _valid )
    {
        _position       = glGetAttribLocation(_programId, "_position");
        _uv             = glGetAttribLocation(_programId, "_uv");
        _deltaUv        = glGetUniformLocation(_programId, "_deltaUv");
        _textureBg      = glGetUniformLocation(_programId, "_textureBg");
        _textureCloud   = glGetUniformLocation(_programId, "_textureCloud");
        _mvp            = glGetUniformLocation(_programId, "_mvp");
    }

    _textureBgId = loadTexture("images/dog.png");
    _textureCloudId = loadTexture("images/fog.bmp");

    return _valid;
}

MultiTexture* MultiTexture::create()
{
    auto * self = new MultiTexture();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int MultiTexture::loadTexture(const std::string &fileName)
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

void MultiTexture::render()
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
    GLfloat w = 300;
    GLfloat h = 300;

    Vertex ary[] =
    {
        float2{ x, y },         float2(0, 1),
        float2{ x + w, y },     float2(1, 1),
        float2{ x, y + h },     float2(0, 0),
        float2{ x + w, y + h }, float2(1, 0),
    };

    static float incUv = 0;
    incUv += 0.01;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureBgId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _textureCloudId);

    glUniformMatrix4fv(_mvp, 1, false, screenProj.data());

    glUniform1i(_textureBg, 0);
    glUniform1i(_textureCloud, 1);

    glUniform1f(_deltaUv, incUv);
    
    glVertexAttribPointer(_position, 2, GL_FLOAT, false, sizeof(Vertex), ary);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &(ary[0].uv));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    end();
}
NS_END(elloop);