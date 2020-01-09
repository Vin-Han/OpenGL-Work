# version 150 core

uniform sampler2D snowTex ;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor ;

void main ( void ) {
	vec4 color = texture(snowTex, IN.texCoord);
	gl_FragColor = color;

}

