precision lowp float;

attribute   vec3    _position;
uniform     mat4    _mvp;

attribute   vec2    _uv;
varying     vec2    _outUv;

void main() {
    vec4 pos    = vec4(_position.x, _position.y, _position.z, 1);
    gl_Position = _mvp * pos;
    _outUv      = _uv;
}
