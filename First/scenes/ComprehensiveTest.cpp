#include "scenes/ComprehensiveTest.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);
NS_BEGIN(comprehensive_test);

void ComprehensiveTest::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void ComprehensiveTest::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool ComprehensiveTest::init()
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
    _textureAll = createTexture(size._width, size._height);

    _textureDog = loadTexture("images/dog.png");
    _textureSuperman = loadTexture("images/superman.jpg");

    char *pics[] = 
    {
        "images/baby1.png",
        "images/baby2.png",
        "images/baby3.png",
        "images/baby4.png",
        "images/me1.jpg",
    };

    _supriseSize = sizeof (pics) / sizeof (pics[0]);
    _textureSuprises = new unsigned int[_supriseSize];

    for (int i=0; i<_supriseSize; ++i)
    {
        _textureSuprises[i] = loadTexture(pics[i]);
        CELL::float3 offsetI(rand() % 100 - 50, 0, 0);
        _transOffsets.push_back(offsetI);
    }

    _camera._eye = CELL::float3(1, 1, 1);
    _camera._look = CELL::float3(0.5f, -0.4f, -5.5f);
    _camera._up = CELL::float3(0.0f, 1.0f, 0.0f);
    _camera._right = CELL::float3(1.0f, 0.0f, 0.0f);
    _camera._moveSpeed = 20;


    return _valid;
}

ComprehensiveTest* ComprehensiveTest::create()
{
    auto * self = new ComprehensiveTest();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int ComprehensiveTest::loadMipMap(const std::vector<std::string> &fileNames)
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

void ComprehensiveTest::render()
{
   /* static int drawCall = 0;
    if (drawCall > 2) 
    {
        return;
    }

    ++drawCall;*/

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



    // draw copy sub picture.
    Vertex vertex3D[] =
    {
        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    };
    glBindTexture(GL_TEXTURE_2D, _textureDynamic);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
    glUniformMatrix4fv(_mvp, 1, false, mvp.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex3D[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex3D[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex3D) / sizeof (vertex3D[0]));




    // draw rotate dog, translate to x + offsetX.
    Vertex vertex3D2[] =
    {
        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },

        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    };

    matrix4 matRotate;
    static float angle = 0;
    angle += 1;
    matRotate.rotateZ(angle);

    matrix4 trans;
    float offsetX = 10;
    trans.translate(float3(offsetX, 0, 0));

    matrix4 mvpRotate3D = matProj * matView * (trans * matRotate);

    glBindTexture(GL_TEXTURE_2D, _textureDog);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
    glUniformMatrix4fv(_mvp, 1, false, mvpRotate3D.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex3D2[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex3D2[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex3D2) / sizeof (vertex3D2[0]));




    // draw rotate superman, translate to offset.
    matrix4 transSuperman;
    float3 offset(25, 0, 10);
    transSuperman.translate(offset);

    matrix4 rotate3DSuperman = matProj * matView * (transSuperman * matRotate);

    glBindTexture(GL_TEXTURE_2D, _textureSuperman);
    //glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
    glUniformMatrix4fv(_mvp, 1, false, rotate3DSuperman.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex3D2[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex3D2[0].u);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex3D2) / sizeof (vertex3D2[0]));


    for (int i = 0; i < _supriseSize; ++i)
    {
        matrix4 trans;
        float3 offset = _transOffsets[i];
        trans.translate(offset);

        matrix4 rotate3DMVP = matProj * matView * (trans * matRotate);

        glBindTexture(GL_TEXTURE_2D, _textureSuprises[i]);
        glUniformMatrix4fv(_mvp, 1, false, rotate3DMVP.data());
        glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex3D2[0].x);
        glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex3D2[0].u);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex3D2) / sizeof (vertex3D2[0]));
    }


    // draw 2D texture include all things, include itself.
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
    glBindTexture(GL_TEXTURE_2D, _textureAll);
    glUniformMatrix4fv(_mvp, 1, false, mvp2D.data());
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex2D[0].x);
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex2D[0].u);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertex2D) / sizeof (vertex2D[0]));

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);

    end();
}

unsigned int ComprehensiveTest::loadTexture(const std::string &fileName)
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

unsigned int ComprehensiveTest::createTexture(int width, int height)
{
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    return textureId;
}



NS_END(comprehensive_test);
NS_END(elloop);