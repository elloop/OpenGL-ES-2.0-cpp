#include "ShaderProgram.h"
#include "ShaderHelper.h"
#include "TextReader.h"

NS_BEGIN(elloop);

void ShaderProgram_p2c4::begin() {
    int i(0);
    i = 1;
    glUseProgram(_programId);
}

void ShaderProgram_p2c4::end() {
    glUseProgram(0);
}


/************************************************************************/
/* implementation of       ShaderProgram_p2c4                                                               */
/************************************************************************/

const std::string ShaderProgram_p2c4::sc_VertexShaderName = "shaders/p2c4_vertex_shader.glsl";
const std::string ShaderProgram_p2c4::sc_FragmentShaderName = "shaders/p2c4_fragment_shader.glsl";

void ShaderProgram_p2c4::initialize() {
    using elloop::TextReader;
    using elloop::ShaderHelper;

    // load shader src.
    _vertexShaderSrc = TextReader::readFromFile(sc_VertexShaderName);
    _vertexShaderId  = ShaderHelper::compileVertexShader(_vertexShaderSrc.c_str());

    _fragShaderSrc = TextReader::readFromFile(sc_FragmentShaderName);
    _fragShaderId  = ShaderHelper::compileFragmentShader(_fragShaderSrc.c_str());

    _programId = ShaderHelper::linkProgram(_vertexShaderId, _fragShaderId);

}

NS_END(elloop);