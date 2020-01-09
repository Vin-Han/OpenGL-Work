#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void) {
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position.xy, 0.0, 1.0);
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
}