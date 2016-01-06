precision lowp float;

varying     vec2        _outUv;
uniform     sampler2D   _textureBg;


void main() 
{
    vec4    bgColor    = texture2D(_textureBg, _outUv);
    if (bgColor.a < 0.2 )
    {
        discard;
    }
    gl_FragColor       = bgColor;
}
