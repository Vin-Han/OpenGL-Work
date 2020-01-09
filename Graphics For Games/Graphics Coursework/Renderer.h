# pragma once

#include"../../nclgl/OGLRenderer.h"
#include"../../nclgl/Camera.h"
#include "../../nclgl/sceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include"../../nclgl/HeightMap.h"
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
# include "../../nclgl/Frustum.h"

#include <algorithm> 
#include <time.h>

#include "SnowSystem.h"

#define MAP_CHANGE_SPEED		0.005f
#define SKY_HEIGHT		400.0f
#define PARTICLE_SCALE		8
#define NEWWATREMOVESPEED		2.0f
#define ROTATESPEED	    3000.0f

# define POST_PASSES 10
# define pixel_Size 1.0f
# define map_Size 1

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f

const int NUM_OF_PARTICLES = 2500;
static int seed = time(NULL);

# define SHADOWSIZE 2048


class Renderer : public OGLRenderer {
public:

	Renderer(Window& parent);
	virtual ~Renderer(void);

#pragma region SceneOperate
	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	int changeScene;
	int openMinimap;
	int openProcess;
#pragma endregion

protected:

#pragma region Light
	void LoadLight();
	void UpdateLight();
	void Renderer::SetMoonLight(const Light& light);
	Light* sunLight;
	Light* moonLight;
	float timeCos;
#pragma endregion

#pragma region Camera
	void LoadCamera();
	void UpdateMyCamera(float msec);
	Camera* freeCamera;
	Camera* miniMapCamera;
	Camera* autoCamera;
	float changeCheck;
	int changeCamera;
#pragma endregion

#pragma region Skybox
	void LoadSkybox();
	void DrawSkybox();
	void UpdateSkybox();
	GLuint cubeMap;
	GLuint cubeMapStar;
	Mesh* cubeMesh;
	Shader* skyboxShader;
	float starRotation;
	float starRatio;
	float addSpe;
#pragma endregion

#pragma region Water
	void LoadWater();
	void DrawWater();
	Mesh* waterMesh;
	Shader* waterShader;
	float waterRotate;
	Matrix4 textureMatrixForStar;
	GLuint waterDudv;
#pragma endregion

#pragma region NewWater
	void LoadNewWater();
	void DrawNewWater();
	void UpdateNewWater();
	HeightMap* NewWaterMesh;
	Shader* NewWaterShader;
	float newwaterRotate;
	float newwaterTemp;
	float newWaterAddDir;
#pragma endregion

#pragma region House
	void LoadHouse(std::string OBJname);
	void DrawHouse(SceneNode* sceneRoot);
	SceneNode* houseNode;
	Mesh* sceneMesh;
	Shader* houseShader;
#pragma endregion

#pragma region Master
	void LoadMaster();
	void DrawMaster();
	MD5FileData* masterData;
	MD5Node* masterNode;
	Shader* masterShader;
#pragma endregion

#pragma region Heightmap
	void LoadHeightmap();
	void DrawHeightmap();
	void UpdateHeightmap();
	HeightMap* heightMap;
	Shader* heightmapShader;
	float mapSpeed;
	float addDir;
#pragma endregion

#pragma region Particle
	void LoadParticle();
	void DrawParticle();
	void UpdateParticle(float msec);

	Shader* showShader;
	SnowSystem* ParticleSystem[NUM_OF_PARTICLES];
	GLuint snowTex;
	Vector3 particleScale;
	GLuint snowColourTex;
	GLuint snowFBO;
	Frustum frameFrustum;
#pragma endregion

#pragma region MiniMap
	void LoadMiniMap();
	void DrawMiniMap();
	Shader* miniMapShader;
	GLuint miniMapFBO;
	GLuint miniMapColourTex;
	GLuint miniMapDepthTex;
	Mesh* miniMapMesh;
#pragma endregion

#pragma region Planet
	void LoadPlanet(std::string OBJname);
	void DrawPlanet(SceneNode* sceneRoot);
	SceneNode* planetNode;
	Mesh* planetMesh;
	Shader* planetShader;
	Vector3 positionChange;
#pragma endregion


#pragma region Sphere
	void LoadSphere(std::string OBJname);
	void DrawSphere(SceneNode* sceneRoot);
	SceneNode* sphereNode;
	GLuint sphereMap;
	Mesh* sphereMesh;
	Shader* sphereShader;
#pragma endregion

#pragma region RealWater
	void LoadRealWater();
	void DrawRealWater();
	GLuint RealWaterFBO;
	GLuint RealWaterColourTex;
	GLuint RealWaterDepthTex;
#pragma endregion

#pragma region SkyboxTwo
	void LoadSkyboxTwo();
	void DrawSkyboxTwo();
	GLuint cubeMapTwo;
	Mesh* cubeMeshTwo;
	Shader* skyboxShaderTwo;

#pragma endregion

#pragma region Heightmap
	void LoadHeightmapTwo();
	void DrawHeightmapTwo();
	void UpdateHeightmapTwo();
	HeightMap* heightMapTwo;
	Shader* heightmapShaderTwo;
	float changeSpeed;
	float flowSpeed;
	float addDirTwo;
#pragma endregion

#pragma region SphereTwo
	void LoadSphereTwo(std::string OBJname);
	void DrawSphereTwo(SceneNode* sceneRoot);
	SceneNode* sphereNodeTwo;
	GLuint sphereMapTwo;
	Mesh* sphereMeshTwo;
	Shader* sphereShaderTwo;
#pragma endregion

#pragma region RocketTwo
	void LoadRocket(std::string OBJname);
	void DrawRocket(SceneNode* sceneRoot);
	SceneNode* RocketNode;
	Mesh* RocketMesh;
	Shader* RocketShader;
	Vector3 RocketPositon;
#pragma endregion

#pragma region Shadow
	void LoadShadow();
	void DrawShadow();
	void DrawCombineShadow();
	void DrawMesh();
	void DrawFloor();

	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData* hellData;
	MD5Node* hellNode;
	Mesh* floor;
#pragma endregion

#pragma region BlurFBO
	void LoadBlur();
	void DrawBlur();
	Shader* BlurShader;
	GLuint BlurFBO;
	GLuint BlurColourTex;
	GLuint BlurDepthTex;
	Mesh* BlurMesh;
#pragma endregion

#pragma region EdgeFBO
	void LoadEdge();
	void DrawEdge();
	Shader* EdgeShader;
	GLuint EdgeFBO;
	GLuint EdgeColourTex;
	GLuint EdgeDepthTex;
	Mesh* EdgeMesh;
#pragma endregion

#pragma region BlurNewFBO
	void LoadBlurNew();
	void DrawBlurNew();
	Shader* BlurNewShader;
	GLuint BlurNewFBO;
	GLuint BlurNewColourTex;
	GLuint BlurNewDepthTex;
	Mesh* BlurNewMesh;
#pragma endregion
};
