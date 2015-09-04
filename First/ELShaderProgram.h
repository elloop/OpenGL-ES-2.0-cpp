#pragma once
#include "GLES2/gl2.h"
#include "elloop/inc.h"
#include "ELRef.h"
#include "ELDrawable.h"

NS_BEGIN(elloop);

typedef GLuint  ShaderId;
typedef GLuint  ProgramId;
typedef int     attribute;
typedef int     uniform;

class ShaderProgram : public Ref, public Drawable
{
public:
    ShaderProgram(const ShaderProgram& other)                    = delete;
    ShaderProgram(ShaderProgram&& other)                         = delete;
    ShaderProgram& operator = (const class ShaderProgram& other) = delete;

    virtual void    begin();
    virtual void    end();
    virtual void    render() override {}
protected:
    ShaderProgram() :
        _vertexShaderId(0),
        _fragShaderId(0),
        _programId(0),
        _vertexShaderSrc(""),
        _fragShaderSrc("")
    {
    }
    virtual         ~ShaderProgram() {}
    virtual bool    initWithFile(const std::string & vsFileName, const std::string & fsFileName);
    virtual bool    validateProgram();

    ShaderId    _vertexShaderId;
    ShaderId    _fragShaderId;
    ProgramId   _programId;
    std::string _vertexShaderSrc;
    std::string _fragShaderSrc;
};


NS_END(elloop);
