#pragma once
#include "GLES2/gl2.h"
#include "elloop/inc.h"
#include "basic/ELRef.h"
#include "ELDrawable.h"

NS_BEGIN(elloop);

typedef GLuint  ShaderId, ProgramId;
typedef int     attribute;
typedef int     uniform;

class ShaderProgram : public Drawable
{
public:
    ShaderProgram(const ShaderProgram& other)                    = delete;
    ShaderProgram(ShaderProgram&& other)                         = delete;
    ShaderProgram& operator = (const class ShaderProgram& other) = delete;

    virtual void    begin() = 0;
    virtual void    end() = 0;
    virtual void    render() override {}
    bool            valid() const { return _valid; }

protected:
    ShaderProgram() 
        : _vertexShaderId(0)
        , _fragShaderId(0)
        , _programId(0)
        , _vertexShaderSrc("")
        , _fragShaderSrc("")
        , _vsFileName("")
        , _fsFileName("")
        , _valid(false)
    {
    }
    virtual         ~ShaderProgram() {}

    virtual bool    initWithFile(const std::string & vsFileName, 
        const std::string & fsFileName);

    virtual bool    validateProgram();

    ShaderId        _vertexShaderId;
    ShaderId        _fragShaderId;
    ProgramId       _programId;
    std::string     _vertexShaderSrc;
    std::string     _fragShaderSrc;
    std::string     _vsFileName;
    std::string     _fsFileName;
    bool            _valid;

};


NS_END(elloop);
