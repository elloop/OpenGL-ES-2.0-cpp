precision lowp float;

attribute   vec2    _position;
attribute   vec2    _uv;
varying     vec2    _outUv;
uniform     mat4    _mvp;

void main() 
{
    vec4 pos     = vec4(_position, 0, 1);
    gl_Position  = _mvp * pos;
    _outUv       = _uv;
}
