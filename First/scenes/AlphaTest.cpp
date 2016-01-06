#include "scenes/AlphaTest.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);
NS_BEGIN(alpha);

void AlphaTest::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void AlphaTest::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool AlphaTest::init()
{
    _valid = ShaderProgram::initWithFile(_vsFileName, _fsFileName);
    if ( _valid )
    {
        _position       = glGetAttribLocation(_programId, "_position");
        _uv             = glGetAttribLocation(_programId, "_uv");
        _textureBg      = glGetUniformLocation(_programId, "_textureBg");
        _mvp            = glGetUniformLocation(_programId, "_mvp");
    }

    std::vector<std::string> fileNames =
    {
        "images/p32X32.bmp",
        "images/p16X16.bmp",
        "images/p8x8.bmp",
        "images/p4x4.bmp",
        "images/p2X2.bmp",
        "images/p1x1.bmp"
    };

    _textureBgId = loadMipMap(fileNames);

    Size size = Director::getInstance()->getFrameSize();
    _textureDynamic = createTexture(size._width, size._height);

    _textureDog = loadTexture("images/grass.png");

    _camera._eye = CELL::float3(1, 1, 1);
    _camera._look = CELL::float3(0.5f, -0.4f, -5.5f);
    _camera._up = CELL::float3(0.0f, 1.0f, 0.0f);
    _camera._right = CELL::float3(1.0f, 0.0f, 0.0f);
    _camera._moveSpeed = 10;


    return _valid;
}

AlphaTest* AlphaTest::create()
{
    auto * self = new AlphaTest();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int AlphaTest::loadMipMap(const std::vector<std::string> &fileNames)
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

void AlphaTest::render()
{
    using namespace CELL;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto director = Director::getInstance();
    Size s = director->getFrameSize();
    float width = s._width;
    float height = s._height;

    glViewport(0, 0, width, height);

    _camera.updateCamera(0.016);

    
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

    // draw ground.
    glUniform1i(_textureBg, 0);
    glBindTexture(GL_TEXTURE_2D, _textureBgId);
    glUniformMatrix4fv(_mvp, 1, false, mvp.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &ground[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &ground[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(ground) / sizeof (ground[0]));



    // draw rotate dog, translate to x + offsetX.
    Vertex vertex3D[] =
    {
        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    };

    matrix4 matTrans;
    matTrans.translate(0, 0, -1);
    matrix4 mvpDog = matProj * matView * matTrans;


    // first dog.
    glBindTexture(GL_TEXTURE_2D, _textureDog);
    glUniformMatrix4fv(_mvp, 1, false, mvpDog.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex3D[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex3D[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex3D) / sizeof (vertex3D[0]));

    // second dog.
    matTrans.translate(-1, 0, -2);
    mvpDog = matProj * matView * matTrans;
    glUniformMatrix4fv(_mvp, 1, false, mvpDog.data());
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex3D) / sizeof (vertex3D[0]));


    // draw 2D texture include all things.
    matrix4 mvp2D = ortho<float>(0, width, height, 0, -100, 100);
    float x = 0;
    float y = 0;
    float w = 200;
    float h = 200;

    // color vector is not used.
    Vertex vertex2D[] =
    {
        x, y, 0, 0, 1, 1, 0, 0, 1,
        x + w, y, 0, 1, 1, 1, 0, 0, 1,
        x, y + h, 0, 0, 0, 1, 0, 0, 1,
        x + w, y + h, 0, 1, 0, 1, 0, 0, 1,
    };
    glBindTexture(GL_TEXTURE_2D, _textureDynamic);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
    glUniformMatrix4fv(_mvp, 1, false, mvp2D.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex2D[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex2D[0].u);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertex2D) / sizeof (vertex2D[0]));


    end();
}

unsigned int AlphaTest::loadTexture(const std::string &fileName)
{
    unsigned int textureId = 0;

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);

    FIBITMAP *bitmap = FreeImage_Load(format, fileName.c_str(), 0);

    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);

    BYTE *pixels = FreeImage_GetBits(bitmap);

    int pixelFormat = GL_RGB;
    if (FIF_PNG == format)
    {
        bitmap = FreeImage_ConvertTo32Bits(bitmap);
        pixelFormat = GL_RGBA;
        for (size_t i = 0; i < width*height * 4; i += 4)
        {
            BYTE temp = pixels[i];
            pixels[i] = pixels[i + 2];
            pixels[i + 2] = temp;
        }
    }
    else
    {
        bitmap = FreeImage_ConvertTo24Bits(bitmap);
        for (size_t i = 0; i < width*height * 3; i += 3)
        {
            BYTE temp = pixels[i];
            pixels[i] = pixels[i + 2];
            pixels[i + 2] = temp;
        }
    }

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, pixels);

    FreeImage_Unload(bitmap);

    return textureId;
}

unsigned int AlphaTest::createTexture(int width, int height)
{
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    return textureId;
}



NS_END(alpha);
NS_END(elloop);