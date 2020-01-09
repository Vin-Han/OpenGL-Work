# version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform samplerCube cubeTexStar;
uniform sampler2D dudvMap;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;
uniform vec4 moonlightColour;
uniform vec3 moonlightPos;
uniform float moonlightRadius;
uniform float starRatio;

in Vertex{
vec2 texCoord;
vec3 normal;
vec3 worldPos;
vec3 starNormal;
} IN;

out vec4 fragColour;

vec4 lightCal(vec4 diffuse, vec3 viewDir, vec4 lightColour, vec3 lightPos, float lightRadius, vec3 worldPos, vec3 normal);

void main(void) {
	vec3 worldPos = IN.worldPos;
	vec2 texCoord = IN.texCoord;
	vec3 normal = IN.normal;

	vec2 dudvOffset = (texture(dudvMap, texCoord)).rg * 2.0 - 1.0;
	//texCoord += dudvOffset * 3 * starRatio;


	vec4 diffuse = texture (diffuseTex, texCoord);
	vec3 viewDir = normalize(cameraPos - worldPos);

	vec4 resultLightone = lightCal(diffuse, viewDir, lightColour, lightPos, lightRadius, worldPos, normal);
	vec4 resultLightMoon = lightCal(diffuse, viewDir, moonlightColour, moonlightPos, moonlightRadius, worldPos, normal);

	vec3 reflectDir = normalize(worldPos - cameraPos);

	vec4 reflection = texture (cubeTex, reflect(reflectDir, normalize(IN.normal)));
	reflection.rgb = reflection.rgb + (diffuse.rgb * lightColour.rgb) * 0.1 + (diffuse.rgb * moonlightColour.rgb) * 0.1;

	vec3 reflectStarDir = normalize(worldPos - cameraPos);
	vec4 reflectionStar = texture (cubeTexStar, reflect(reflectStarDir, normalize(IN.starNormal)));
	reflectionStar.rgb = reflectionStar.rgb + (diffuse.rgb * lightColour.rgb) * 0.1 + (diffuse.rgb * moonlightColour.rgb) * 0.1;


	fragColour = reflection + reflectionStar * starRatio + resultLightone  + resultLightMoon;
}

vec4 lightCal(vec4 diffuse, vec3 viewDir, vec4 lightColour, vec3 lightPos, float lightRadius, vec3 worldPos, vec3 normal) {

	vec3 colour = (diffuse.rgb * lightColour.rgb);
	vec3 incident = normalize(lightPos - worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	float Factor = pow(max(0.0, dot(halfDir, normal)), 50.0);
	float lambert = max(0.0, dot(incident, normal));
	float dist = length(lightPos - worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	colour += (lightColour.rgb * Factor) * 0.66;
	vec4 resultLight = vec4(colour * atten * lambert, diffuse.a);
	resultLight.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
	return resultLight;
}