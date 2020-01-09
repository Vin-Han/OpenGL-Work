# version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform vec3 mapSpeed;

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

	vec3 temp = mapSpeed * position;

	gl_Position = (projMatrix * viewMatrix * modelMatrix) *
		vec4(temp, 1.0);

}