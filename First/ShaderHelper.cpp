#include "ShaderHelper.h"
#include "Config.h"
#include "gltest.h"

NS_BEGIN(elloop);

/************************************************************************/
/* related OpenGL ES 2.0 API                                            */
/************************************************************************/

/*

GLuint glCreateShader(	GLenum shaderType); // returns 0 when fail.


void glGetShaderiv(GLuint shader,
    GLenum pname,
    GLint *params); // return GL_TRUE or GL_FALSE


GLuint glCreateProgram(	void); // returns 0 when fail.

*/


const ShaderId ShaderHelper::compileShader(const GLchar* shaderSrc, GLenum shaderType) {
    // create shader.
    ShaderId shaderId = glCreateShader(shaderType);
    assert(shaderId != 0);

    // attach source.
    glShaderSource(shaderId, 1, &shaderSrc, NULL);

    // compile.
    glCompileShader(shaderId);

    // check the compile result.
    GLint compileStatus(0);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
    if (GL_FALSE == compileStatus) {
        glDeleteShader(shaderId);
        shaderId = 0;
    }
    assert(compileStatus == GL_TRUE);

    return shaderId;
}

ProgramId ShaderHelper::linkProgram(ShaderId vertexShaderId, ShaderId fragShaderId) {
    ProgramId programId = glCreateProgram();
    assert(programId != 0);
    if (!programId && LOG_OPEN) {
        GLchar  errorMsg[256];
        GLsizei errorLength(0);
        // todo: check params valid? and log.
        glGetProgramInfoLog(programId, 256, &errorLength, errorMsg);
        // log
        return 0;
    }
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);
    GLint linkStatus(0);
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (GL_FALSE == linkStatus) {
        glDeleteProgram(programId);
        programId = 0;
    }
    assert(linkStatus == GL_TRUE);
    return programId;
}

NS_END(elloop);