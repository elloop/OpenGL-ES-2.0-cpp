#pragma once
#include "GLES2/gl2.h"
#include "elloop/inc.h"
#include "Ref.h"

NS_BEGIN(elloop);

typedef GLuint  ShaderId;
typedef GLuint  ProgramId;
typedef int     attribute;
typedef int     uniform;

class ShaderProgram : public Ref
{
public:
    ShaderProgram(const ShaderProgram& other)                    = delete;
    ShaderProgram(ShaderProgram&& other)                         = delete;
    ShaderProgram& operator = (const class ShaderProgram& other) = delete;

    virtual void begin();
    virtual void end();
protected:
    ShaderProgram() 
        :   _vertexShaderId(0),
            _fragShaderId(0),
            _programId(0),
            _vertexShaderSrc(""),
            _fragShaderSrc("")
    {
    }
    virtual ~ShaderProgram() {}
    virtual bool initWithFile(const std::string & vsFileName, const std::string & fsFileName);
    virtual bool validateProgram();

    ShaderId    _vertexShaderId;
    ShaderId    _fragShaderId;
    ProgramId   _programId;
    std::string _vertexShaderSrc;
    std::string _fragShaderSrc;
};

class ShaderProgram_p2c4 : public ShaderProgram {
public:
    static ShaderProgram_p2c4*  create();
    bool                        valid() const { return _valid;}
    void                        begin() override;
    void                        end()   override;

    attribute                   _position;
    uniform                     _mvp;
    uniform                     _color;
protected:
    bool init();
    ShaderProgram_p2c4() {}
    ~ShaderProgram_p2c4() {
    
    }
private:
    bool                        _valid;
    static const std::string    sc_VertexShaderName;
    static const std::string    sc_FragmentShaderName;
};

NS_END(elloop);
