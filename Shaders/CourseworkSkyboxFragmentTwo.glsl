#version 150 core

uniform samplerCube cubeMapTwo;

in Vertex{
	vec3 normal;
}IN;

out vec4 fragColor;

void main(void) {
	vec4 baseColor = texture(cubeMapTwo, normalize(IN.normal));

	fragColor = baseColor;

}