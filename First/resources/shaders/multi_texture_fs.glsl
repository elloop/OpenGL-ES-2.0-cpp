precision lowp float;

varying     vec2        _outUv;
uniform     sampler2D   _textureBg;
uniform     sampler2D   _textureCloud;
uniform     float       _deltaUv;


void main() 
{
    vec4    bgColor    = texture2D(_textureBg, _outUv);
    vec2    moveUv     = vec2(_outUv.x + _deltaUv, _outUv.y);
    vec4    cloudColor = texture2D(_textureCloud, moveUv);
    gl_FragColor    = bgColor + cloudColor;
    /* gl_FragColor       = mix(bgColor, cloudColor, 0.5); */
}
