# version 150 core
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;


out Vertex{
vec3 normal;
vec3 starNormal;
} OUT;

void main(void) {

	vec3 multiply = vec3(0, 0, 0);
	multiply.x = 1.0f / projMatrix[0][0];
	multiply.y = 1.0f / projMatrix[1][1];

	vec3 tempPos = (position * multiply) - vec3(0, 0, 1);
	OUT.normal = transpose(mat3(viewMatrix)) * normalize(tempPos);

	vec3 temp = transpose(mat3(viewMatrix)) * normalize(tempPos);
	vec4 startemp = textureMatrix * vec4(temp.xyz, 1.0);
	OUT.starNormal = vec3(startemp.xyz);

	gl_Position = vec4(position, 1.0);
}