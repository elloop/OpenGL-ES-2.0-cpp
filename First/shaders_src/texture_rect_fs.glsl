precision lowp float;
varying     vec4        _outputColor;
varying     vec2        _outUv;
uniform     sampler2D   _texutreId;


void main() {
    vec4    tColor = texture2D(_texutreId, _outUv);
	gl_FragColor = tColor * _outputColor;
}
