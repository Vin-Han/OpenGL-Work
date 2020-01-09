# version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform float changeSpeed;
uniform float flowSpeed;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex{
vec4 colour;
vec2 texCoord;
vec3 normal;
vec3 worldPos;
vec3 tangent;
vec3 binormal;
} OUT;

void main(void) {
	OUT.colour = colour;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT.normal = normalize(normalMatrix * normalize(normal));

	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

	OUT.tangent = normalize(normalMatrix * normalize(tangent));

	OUT.binormal = normalize(normalMatrix *
		normalize(cross(normal, tangent)));

	float tempX = cos((position.z/100 + changeSpeed))*100;
	float tempY = cos((position.x / 100 + changeSpeed))*100;

	float mixResult = mix(tempY , tempX, 0.5);

	vec3 tempPos = vec3(position.x , mixResult * flowSpeed, position.z);

	gl_Position = (projMatrix * viewMatrix * modelMatrix) *
		vec4(tempPos, 1.0);

}