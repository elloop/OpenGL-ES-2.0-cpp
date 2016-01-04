precision lowp float;

attribute   vec2    position_;
uniform     mat4    mvp_;

attribute   vec2    uv_;
varying     vec2    outUv_;

void main() {
    vec4 pos    = vec4(position_, 0, 1);
    gl_Position = mvp_ * pos;
    outUv_      = uv_;
}
