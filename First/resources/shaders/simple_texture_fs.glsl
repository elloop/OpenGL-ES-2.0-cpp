precision lowp float;

varying     vec2        _outUv;
uniform     sampler2D   _texture;

void main() 
{
    gl_FragColor = texture2D(_texture, _outUv);
}
