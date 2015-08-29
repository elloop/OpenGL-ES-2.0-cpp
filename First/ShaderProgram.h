#pragma once
#include "GLES2/gl2.h"
#include "elloop/inc.h"
NS_BEGIN(elloop);

typedef GLuint ShaderId;
typedef GLuint ProgramId;

class ShaderProgram
{
public:
    virtual void initialize() = 0;
    virtual void begin() = 0;
    virtual void end() = 0;
protected:
    ShaderId    _vertexShaderId;
    ShaderId    _fragShaderId;
    ProgramId   _programId;
    std::string _vertexShaderSrc;
    std::string _fragShaderSrc;
};

class ShaderProgram_p2c4 : public ShaderProgram {
public:
    void initialize() override;
    void begin() override;
    void end() override;
private:
    static const std::string    sc_VertexShaderName;
    static const std::string    sc_FragmentShaderName;
};

NS_END(elloop);
