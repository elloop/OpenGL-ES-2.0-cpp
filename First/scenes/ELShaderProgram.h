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
    bool            valid() const { return valid_; }

protected:
    ShaderProgram() 
        : vertexShaderId_(0)
        , fragShaderId_(0)
        , programId_(0)
        , vertexShaderSrc_("")
        , fragShaderSrc_("")
        , vsFileName_("")
        , fsFileName_("")
        , valid_(false)
    {
    }
    virtual         ~ShaderProgram() {}

    virtual bool    initWithFile(const std::string & vsFileName, 
        const std::string & fsFileName);

    virtual bool    validateProgram();

    ShaderId        vertexShaderId_;
    ShaderId        fragShaderId_;
    ProgramId       programId_;
    std::string     vertexShaderSrc_;
    std::string     fragShaderSrc_;
    std::string     vsFileName_;
    std::string     fsFileName_;
    bool            valid_;

};


NS_END(elloop);
