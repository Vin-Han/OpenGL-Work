# version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube sphereMap;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;
uniform vec3 bumpTex;
uniform vec4 moonlightColour;
uniform vec3 moonlightPos;
uniform float moonlightRadius;

in Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProj;
} IN;

out vec4 fragColour;

vec4 lightCal(vec4 diffuse, vec3 viewDir, vec4 lightColour, vec3 lightPos, float lightRadius, vec3 worldPos, vec3 normal);

void main(void) {
	vec3 worldPos = IN.worldPos;
	vec2 texCoord = IN.texCoord;
	vec3 normal = IN.normal;
	//vec4 diffuse = texture (diffuseTex, texCoord);
	vec4 diffuse =  texture (sphereMap, normalize(IN.normal));
	vec3 viewDir = normalize(cameraPos - worldPos);

	vec4 resultLightone = lightCal(diffuse, viewDir, lightColour, lightPos, lightRadius, worldPos, normal);
	vec4 resultLightMoon = lightCal(diffuse, viewDir, moonlightColour, moonlightPos, moonlightRadius, worldPos, normal);

	fragColour = diffuse + resultLightone + resultLightMoon;
	//fragColour = resultLightone  + resultLightMoon;
}

vec4 lightCal(vec4 diffuse, vec3 viewDir, vec4 lightColour, vec3 lightPos, float lightRadius, vec3 worldPos, vec3 normal) {

	vec3 colour = (diffuse.rgb * lightColour.rgb);
	vec3 incident = normalize(lightPos - worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	float Factor = pow(max(0.0, dot(halfDir, normal)), 50.0);
	float lambert = max(0.0, dot(incident, normal));

	//float shadow = 1.0;
	//if (IN.shadowProj.w > 0.0) { shadow = textureProj(shadowTex, IN.shadowProj); }
	//lambert *= shadow;

	float dist = length(lightPos - worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	colour += (lightColour.rgb * Factor) * 0.66;
	vec4 resultLight = vec4(colour * atten * lambert, diffuse.a);
	resultLight.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
	return resultLight;
}