
#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec2      varTexcoord;
out vec4     fragColor;
#else
varying vec2 varTexcoord;
#endif

uniform sampler2D diffuseTexture;


void main (void)
{
	#if __VERSION__ >= 140
	fragColor = texture(diffuseTexture, varTexcoord.st, 0.0);
	#else
    gl_FragColor = texture2D(diffuseTexture, varTexcoord.st, 0.0);
	#endif
}