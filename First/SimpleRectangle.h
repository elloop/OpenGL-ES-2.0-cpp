#pragma  once

#include "gltest.h"
#include "ELShaderProgram.h"

NS_BEGIN(elloop);


class SimpleRectangle : public ShaderProgram {
public:
    static SimpleRectangle*     create();
    bool                        valid() const { return _valid; }
    void                        begin() override;
    void                        end()   override;
    void                        render() override;

    attribute                   _position;
    uniform                     _mvp;
    uniform                     _color;
protected:
    bool init();
    SimpleRectangle() {}
    ~SimpleRectangle() {

    }
private:
    bool                        _valid;
    static const std::string    sc_VertexShaderName;
    static const std::string    sc_FragmentShaderName;
};



NS_END(elloop);