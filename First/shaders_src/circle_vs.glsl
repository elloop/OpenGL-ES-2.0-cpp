precision lowp float;
attribute vec2  _position;
attribute vec4  _color;
varying   vec4  _outputColor;
uniform mat4    _mvp;

void main() {
    vec4 pos = vec4(_position, 0, 1);
    gl_Position = _mvp * pos;
    _outputColor = _color;
}
