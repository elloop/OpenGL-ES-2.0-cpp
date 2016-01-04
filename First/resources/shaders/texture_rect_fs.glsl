precision lowp float;
varying     vec2        outUv_;
uniform     sampler2D   texture_;

void main() 
{
    gl_FragColor = texture2D(texture_, outUv_);
}
