#version 150
uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform vec2 pixelSize;

in Vertex{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 FragColor;


void main(void) {
	FragColor = texture(diffuseTex, IN.texCoord);
	FragColor.x = 1.0 - FragColor.x;
	FragColor.y = 1.0 - FragColor.y;
	FragColor.z = 1.0 - FragColor.z;
}