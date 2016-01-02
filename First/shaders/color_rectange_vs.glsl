precision lowp float;
attribute vec2  _position;
uniform mat4    _mvp;

void main() {
    vec4 pos = vec4(_position, 0, 1);
    gl_Position = _mvp * pos;
}