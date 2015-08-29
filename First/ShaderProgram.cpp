#include "ShaderProgram.h"
#include "GLES2/gl2.h"

NS_BEGIN(elloop);

ShaderProgram::ShaderProgram() {
}


ShaderProgram::~ShaderProgram() {
}

void ShaderProgram::initialize() {
}

void ShaderProgram::begin() {
}

void ShaderProgram::end() {
}


void ShaderProgram_p2c4::begin() {
    glUseProgram(_programId);
}

void ShaderProgram_p2c4::end() {
    glUseProgram(0);
}

NS_END(elloop);