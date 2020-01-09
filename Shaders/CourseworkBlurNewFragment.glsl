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

void main(void){


	vec4 color = vec4(0.0);
	int coreSize = 3;
	float texelOffset = 1 / 150.0;
	float kernel[9];

	kernel[6] = 1.5; kernel[7] = 2.0; kernel[8] = 1.5;
	kernel[3] = 2; kernel[4] = 2.5; kernel[5] = 2.0;
	kernel[0] = 1.5; kernel[1] = 2.0; kernel[2] = 1.5;


	int index = 0;
	for (int y = 0; y < coreSize; y++)
	{
		for (int x = 0; x < coreSize; x++)
		{
			vec4 currentColor = texture2D(diffuseTex, IN.texCoord.xy + vec2((-1 + x) * texelOffset, (-1 + y) * texelOffset));
			color += currentColor * kernel[index++];
		}
	}
	color /= 16.0;
	FragColor = color;

}