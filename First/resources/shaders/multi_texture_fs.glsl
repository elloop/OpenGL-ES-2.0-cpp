precision lowp float;

varying     vec2        outUv_;
uniform     sampler2D   textureBg_;
uniform     sampler2D   textureCloud_;
uniform     float       uvAnim_;


void main() 
{
    vec4    bgColor    = texture2D(textureBg_, outUv_);
    vec2    newUv      = vec2(outUv_.x + uvAnim_, outUv_.y);
    vec4    cloudColor = texture2D(textureCloud_, newUv);
    gl_FragColor       = bgColor + cloudColor;
    /* gl_FragColor       = bgColor * cloudColor; */
    /* gl_FragColor       = mix(bgColor, cloudColor, 0.5); */
}
