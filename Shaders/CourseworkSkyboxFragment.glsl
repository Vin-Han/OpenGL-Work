#version 150 core

uniform samplerCube cubeTex;
uniform samplerCube cubeTexStar;

uniform vec3 cameraPos;
uniform float starRatio;

in Vertex{
	vec3 normal;
	vec3 starNormal;
}IN;

out vec4 fragColor;

void main(void) {
	vec4 baseColor = texture(cubeTex, normalize(IN.normal));
	vec4 starColor = texture(cubeTexStar, normalize(IN.starNormal));
	fragColor = baseColor + starColor * starRatio;

}