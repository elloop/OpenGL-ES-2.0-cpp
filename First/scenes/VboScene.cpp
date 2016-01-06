#include "scenes/VboScene.h"
#include "app_control/ELDirector.h"
#include "math/ELGeometry.h"
#include "include/freeImage/FreeImage.h"

NS_BEGIN(elloop);
NS_BEGIN(vbo_scene);

void VboScene::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_uv);
}

void VboScene::end()
{
    glDisableVertexAttribArray(_uv);
    glDisableVertexAttribArray(_position);
    glUseProgram(0);
}

bool VboScene::init()
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
        "images/p32x32.bmp",
        "images/p16x16.bmp",
        "images/p8x8.bmp",
        "images/p4x4.bmp",
        "images/p2X2.bmp",
        "images/p1x1.bmp"
    };

    _textureBgId = loadMipMap(fileNames);
    _textureCube = loadTexture("images/1.jpg");

    _camera._eye = CELL::float3(1, 1, 1);
    _camera._look = CELL::float3(0.5f, -0.4f, -5.5f);
    _camera._up = CELL::float3(0.0f, 1.0f, 0.0f);
    _camera._right = CELL::float3(1.0f, 0.0f, 0.0f);

    
    Vertex cubeVertex[] =
    {
        { -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 0
        {  1.0f, -1.0f,  1.0f,  0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 1
        {  1.0f,  1.0f,  1.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 2
        { -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 3
        { -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 4
        { -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 5
        {  1.0f,  1.0f, -1.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 6
        {  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f }, // 7
    };

    GLubyte cubeIndices[] =
    {
        0, 1, 2, 0, 2, 3, // Quad 0
        4, 5, 6, 4, 6, 7, // Quad 1
        5, 3, 2, 5, 2, 6, // Quad 2
        4, 7, 1, 4, 1, 0, // Quad 3
        7, 6, 2, 7, 2, 1, // Quad 4
        4, 0, 3, 4, 3, 5  // Quad 5
    };

    // create vertex buffer object.
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), cubeVertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeIndices ), cubeIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // or
    /* glBufferData(GL_ARRAY_BUFFER, sizeof vertexes, 0, GL_STATIC_DRAW);
     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof vertexes, vertexes);*/

    return _valid;
}

VboScene* VboScene::create()
{
    auto * self = new VboScene();
    if ( self && self->init() )
    {
        self->autorelease();
        return self;
    }
    return nullptr;
}

unsigned int VboScene::loadMipMap(const std::vector<std::string> &fileNames)
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

void VboScene::render()
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



    // draw cube, 
    matrix4 matTrans;
    matTrans.translate(0, 0, -1);
    matrix4 mvpDog = matProj * matView * matTrans;

    glBindTexture(GL_TEXTURE_2D, _textureCube);
    glUniformMatrix4fv(_mvp, 1, false, mvpDog.data());

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    int offset = 0;
    glVertexAttribPointer(_position, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offset));

    offset += 3 * sizeof(float); // u,v is after x,y,z three floats.
    glVertexAttribPointer(_uv, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offset));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0); // use ibo.

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    end();
}

unsigned int VboScene::loadTexture(const std::string &fileName)
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

NS_END(vbo_scene);
NS_END(elloop);