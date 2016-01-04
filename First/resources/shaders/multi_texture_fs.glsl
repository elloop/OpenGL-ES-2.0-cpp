precision lowp float;

varying     vec2        outUv_;
uniform     sampler2D   textureBg_;
uniform     sampler2D   textureCloud_;
uniform     float       deltaUv_;


void main() 
{
    vec4    bgColor    = texture2D(textureBg_, outUv_);
    vec2    moveUv     = vec2(outUv_.x + deltaUv_, outUv_.y);
    vec4    cloudColor = texture2D(textureCloud_, moveUv);
    gl_FragColor    = bgColor + cloudColor;
    /* gl_FragColor       = mix(bgColor, cloudColor, 0.5); */
}
