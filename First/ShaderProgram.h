#pragma once
#include "elloop/inc.h"
#include "GLES2/gl2.h"
NS_BEGIN(elloop);

class ShaderId {
public:
    typedef GLuint ElemType;
    ShaderId(ElemType id=0): _id(id) {}
    operator ElemType() {
        return _id;
    }
private:
    ElemType _id;
};

typedef ShaderId ProgramId;

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    void initialize();
    virtual void begin();
    virtual void end();
protected:
    ShaderId    _vertexShaderId;
    ShaderId    _fragShaderId;
    ProgramId   _programId;
};

class ShaderProgram_p2c4 : public ShaderProgram {
    void begin() override;
    void end() override;
};

NS_END(elloop);
