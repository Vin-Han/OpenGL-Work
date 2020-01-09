#version 150
uniform sampler2D diffuseTex;
uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform vec2 pixelSize;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 FragColor;

const float weights[5] = float[](0.12, 0.22, 0.32, 0.22, 0.12);

void main(void){

	vec2 values[5];

	values = vec2[](vec2(0.0, -pixelSize.y * 2),
		vec2(0.0, -pixelSize.y * 1), vec2(0.0, 0.0),
		vec2(0.0, pixelSize.y * 1), vec2(0.0, pixelSize.y * 2));

	for (int i = 0; i < 5; i++) {
		vec4 tmp = texture2D (diffuseTex, IN.texCoord.xy + values[i]);
		FragColor += tmp * weights[i];
	}
}