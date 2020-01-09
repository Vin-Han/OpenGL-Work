# version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 textureMatrixForStar;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec2 texCoord;

out Vertex{
vec2 texCoord;
vec3 normal;
vec3 worldPos;
vec3 starNormal;
} OUT;

void main(void) {
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));

	vec3 temp = normalize(normalMatrix * normalize(normal));
	vec4 startemp = textureMatrixForStar * vec4(temp.xyz, 1.0);
	OUT.starNormal = vec3(startemp.xyz);

	gl_Position = (projMatrix * viewMatrix * modelMatrix) *
		vec4(position, 1.0);
}