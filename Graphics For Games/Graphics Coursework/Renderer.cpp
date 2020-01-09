#include"Renderer.h"

#pragma region WoringOn



#pragma endregion

#pragma region WoringOn_Problem work

void  Renderer::LoadRealWater() {
	//generate textuer data
	glGenTextures(1, &RealWaterDepthTex);
	glBindTexture(GL_TEXTURE_2D, RealWaterDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &RealWaterColourTex);
	glBindTexture(GL_TEXTURE_2D, RealWaterColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//bind textureattachment
	glGenFramebuffers(1, &RealWaterFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, RealWaterFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, RealWaterDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, RealWaterDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, RealWaterColourTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !RealWaterDepthTex || !RealWaterColourTex) { return; }
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void  Renderer::DrawRealWater() {
	glUseProgram(houseShader->GetProgram());
	float tempCam = freeCamera->GetPosition().y;
	freeCamera->SetYaw(-freeCamera->GetYaw());
	freeCamera->SetPosition(Vector3(freeCamera->GetPosition().x, -freeCamera->GetPosition().y - 932, freeCamera->GetPosition().z));
	viewMatrix = freeCamera->BuildViewMatrix();
	//get current picuture
	glBindFramebuffer(GL_FRAMEBUFFER, miniMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	DrawSkybox();
	DrawHouse(houseNode);
	DrawPlanet(planetNode);
	DrawRocket(RocketNode);
	DrawMaster();
	DrawHeightmap();
	DrawSphere(sphereNode);
	DrawParticle();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	freeCamera->SetYaw(-freeCamera->GetYaw());
	freeCamera->SetPosition(Vector3(freeCamera->GetPosition().x, tempCam, freeCamera->GetPosition().z));
	viewMatrix = freeCamera->BuildViewMatrix();
	modelMatrix.ToIdentity();
	glUseProgram(0);

	////draw processed picture
	//glDisable(GL_DEPTH_TEST);
	//SetCurrentShader(miniMapShader);
	//
	//viewMatrix.ToIdentity();
	//projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	//modelMatrix = Matrix4::Scale(Vector3(0.5, 0.5, 0.5));
	//UpdateShaderMatrices();
	//
	//miniMapMesh->SetTexture(miniMapColourTex);
	//miniMapMesh->Draw();
	//
	//modelMatrix.ToIdentity();
	//projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	//viewMatrix = freeCamera->BuildViewMatrix();
	//glEnable(GL_DEPTH_TEST);
	//glUseProgram(0);
}


#pragma endregion

#pragma region LoadScene

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	LoadLight();
	LoadCamera();
	LoadSkybox();

	LoadWater();
	LoadNewWater();
	LoadHouse(MESHDIR"file.obj");
	LoadMaster();
	LoadHeightmap();
	LoadParticle();
	LoadMiniMap();
	LoadPlanet(MESHDIR"planet.obj");
	LoadRocket(MESHDIR"rock.obj");
	LoadSphere(MESHDIR"sphere.obj");
	LoadBlur();
	LoadEdge();
	LoadBlurNew();

	LoadSkyboxTwo();
	LoadHeightmapTwo();
	LoadSphereTwo(MESHDIR"sphere.obj");

	LoadShadow();

	LoadRealWater();

	//Global Settings
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//update
	init = true;
	changeScene = 1;
	openMinimap = 1;
	openProcess = 4;
}

void Renderer::LoadLight() {

	sunLight = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(1, 1, 1, 1), 16 * (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);

	moonLight = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(1, 0, 0, 1), 10 * (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);

	timeCos = 0.0f;
}

void Renderer::LoadCamera() {
	freeCamera = new Camera(0.0f, 50.0f, Vector3(1500, -300, 2000));
	miniMapCamera = new Camera(270.0f, 0, Vector3(0, 6000, 1000));
	autoCamera = new Camera(0.0f, 50.0f, Vector3(1500, -300, 2000));
	changeCheck = 0.0f;
	changeCamera = 1;
}

void Renderer::LoadSkybox() {
	//mesh
	cubeMesh = Mesh::GenerateQuad();

	//texture
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "grave_ft.tga", TEXTUREDIR "grave_bk.tga",
		TEXTUREDIR "grave_up.tga", TEXTUREDIR "grave_dn.tga",
		TEXTUREDIR "grave_rt.tga", TEXTUREDIR "grave_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	cubeMapStar = SOIL_load_OGL_cubemap(
		TEXTUREDIR "purplenebula_ft.tga", TEXTUREDIR "purplenebula_bk.tga",
		TEXTUREDIR "purplenebula_up.tga", TEXTUREDIR "purplenebula_dn.tga",
		TEXTUREDIR "purplenebula_rt.tga", TEXTUREDIR "purplenebula_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	SetTextureRepeating(cubeMesh->GetTexture(), true);
	//shader
	skyboxShader = new Shader(SHADERDIR "CourseworkSkyboxVertex.glsl",
		SHADERDIR "CourseworkSkyboxFragment.glsl");
	if (!skyboxShader->LinkProgram()) {return;}
	//update
	starRotation = 0;
	starRatio = 0;
	addSpe = MAP_CHANGE_SPEED;
}

void Renderer::LoadWater() {
	//mesh
	waterMesh = Mesh::GenerateQuad();
	//texture
	waterDudv = SOIL_load_OGL_texture(TEXTUREDIR"dudvmap.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	waterMesh->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR "water.TGA",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!waterMesh->GetTexture()) {return;}
	SetTextureRepeating(waterMesh->GetTexture(), true);
	//shader
	waterShader = new Shader(SHADERDIR "CourseworkWaterVertex.glsl",
		SHADERDIR "CourseworkWaterFragment.glsl");
	if (!waterShader->LinkProgram()) {return;}
	//update
	textureMatrixForStar.ToIdentity();
}

void Renderer::LoadNewWater() {
	//mesh
	NewWaterMesh = new HeightMap(TEXTUREDIR"tempHeight.raw");
	//texture
	NewWaterMesh->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR "newwater2.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!NewWaterMesh->GetTexture()) { return; }
	SetTextureRepeating(NewWaterMesh->GetTexture(), true);
	//shader
	NewWaterShader = new Shader(SHADERDIR "CourseworkNewWaterVertex.glsl",
		SHADERDIR "CourseworkNewWaterFragment.glsl");
	if (!NewWaterShader->LinkProgram()) { return; }
	//update
	newwaterRotate = 0.0f;
	newwaterTemp = 0.0f;
	newWaterAddDir = MAP_CHANGE_SPEED;
	//update
	waterRotate = 0.0f;
}

void Renderer::LoadHouse(std::string OBJname) {
	//mesh &texture
	OBJMesh* tempMesh = new OBJMesh();
	tempMesh->LoadOBJMesh(OBJname);
	sceneMesh = tempMesh;

	houseNode = new SceneNode();
	SceneNode* temp = new SceneNode();
	temp->SetMesh(sceneMesh);
	houseNode->AddChild(temp);
	//shader
	houseShader = new Shader(SHADERDIR"CourseworkHouseVertex.glsl",
		SHADERDIR"CourseworkHouseFragment.glsl");
	if (!houseShader->LinkProgram()) {
		return;
	}
}

void Renderer::LoadMaster() {
	//mesh
	masterData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	//texture
	masterNode = new MD5Node(*masterData);
	//animation
	masterData->AddAnim(MESHDIR"idle2.md5anim");
	masterNode->PlayAnim(MESHDIR"idle2.md5anim");
	//shader
	masterShader = new Shader(SHADERDIR "CourseworkMasterVertex.glsl",
		SHADERDIR "CourseworkMasterFragment.glsl");
	if (!masterShader->LinkProgram()) {return;}
}

void Renderer::LoadHeightmap() {
	//mesh
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	//texture
	heightMap->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren Reds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!heightMap->GetTexture() || !heightMap->GetBumpMap()) { return; }
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	//shader
	heightmapShader = new Shader(SHADERDIR "CourseworkHeightVertex.glsl",
		SHADERDIR "CourseworkHeightFragment.glsl");
	if (!heightmapShader->LinkProgram()) { return; }
	//update
	mapSpeed = 0.0f;
	addDir = MAP_CHANGE_SPEED;
}

void Renderer::LoadParticle() {
	//basic set
	particleScale = Vector3(PARTICLE_SCALE, PARTICLE_SCALE, 1);
	int SnowNumRow = sqrt(NUM_OF_PARTICLES);
	//random position function
	auto GetRand = [](int min, int max) -> int { srand(seed); seed = rand(); return (min + rand() % (max - min + 1)); };
	//mesh
	for (int ver = 0; ver < SnowNumRow; ver++) {
		for (int hor = 0; hor < SnowNumRow; hor++) {
			double lifeDuration = double(GetRand(30, 190));
			Vector3 snowVelocity(GetRand(10, 20), GetRand(10, 60), GetRand(10, 20));
			double offSetValue = double(HEIGHTMAP_X * RAW_WIDTH / double(SnowNumRow));

			ParticleSystem[ver * SnowNumRow + hor] = new SnowSystem(Vector3(offSetValue * ver, SKY_HEIGHT, offSetValue * hor),
				snowVelocity, lifeDuration * 1000, particleScale);
		}
	}
	//texture
	snowTex = SOIL_load_OGL_texture(TEXTUREDIR"SnowFlower.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!snowTex) { return; }
	//shader
	showShader = new Shader(SHADERDIR "CourseworkParticleVertex.glsl", SHADERDIR "CourseworkParticleFragment.glsl");
	if (!showShader->LinkProgram()) { return; }
}

void Renderer::LoadMiniMap() {
	//mesh
	miniMapMesh = Mesh::GenerateQuad();
	//shader
	miniMapShader = new Shader(SHADERDIR"CourseworkMiniMapVertex.glsl",
		SHADERDIR"CourseworkMiniMapFragment.glsl");
	if (!miniMapShader->LinkProgram()) { return; }
	//generate textuer data
	glGenTextures(1, &miniMapDepthTex);
	glBindTexture(GL_TEXTURE_2D, miniMapDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &miniMapColourTex);
	glBindTexture(GL_TEXTURE_2D, miniMapColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//bind textureattachment
	glGenFramebuffers(1, &miniMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, miniMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, miniMapDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, miniMapDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, miniMapColourTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=GL_FRAMEBUFFER_COMPLETE || !miniMapDepthTex || !miniMapColourTex) {return;}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::LoadPlanet(std::string OBJname) {
	//mesh &texture
	OBJMesh* tempMesh = new OBJMesh();
	tempMesh->LoadOBJMesh(OBJname);
	planetMesh = tempMesh;
	planetNode = new SceneNode();
	SceneNode* temp = new SceneNode();
	temp->SetMesh(planetMesh);
	planetNode->AddChild(temp);
	//shader
	planetShader = new Shader(SHADERDIR"CourseworkPlanetVertex.glsl",
		SHADERDIR"CourseworkPlanetFragment.glsl");
	if (!planetShader->LinkProgram()) {
		return;
	}
}

void Renderer::LoadSphere(std::string OBJname) {
	//mesh &texture
	OBJMesh* tempMesh = new OBJMesh();
	tempMesh->LoadOBJMesh(OBJname);
	sphereMesh = tempMesh;
	sphereNode = new SceneNode();
	SceneNode* temp = new SceneNode();
	temp->SetMesh(sphereMesh);
	sphereNode->AddChild(temp);
	//texture
	sphereMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "5dim_ft.tga", TEXTUREDIR "5dim_bk.tga",
		TEXTUREDIR "5dim_up.tga", TEXTUREDIR "5dim_dn.tga",
		TEXTUREDIR "5dim_rt.tga", TEXTUREDIR "5dim_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	SetTextureRepeating(cubeMesh->GetTexture(), true);
	//shader
	sphereShader = new Shader(SHADERDIR"CourseworkSphereVertex.glsl",
		SHADERDIR"CourseworkSphereFragment.glsl");
	if (!sphereShader->LinkProgram()) {
		return;
	}
	//update
}

void Renderer::LoadBlur() {
	//mesh
	BlurMesh = Mesh::GenerateQuad();
	//shader
	BlurShader = new Shader(SHADERDIR"CourseworkProcessVertex.glsl",
		SHADERDIR"CourseworkBlurFragment.glsl");

	if (!BlurShader->LinkProgram()) { return; }
	//generate textuer data
	glGenTextures(1, &BlurDepthTex);
	glBindTexture(GL_TEXTURE_2D, BlurDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &BlurColourTex);
	glBindTexture(GL_TEXTURE_2D, BlurColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//bind textureattachment
	glGenFramebuffers(1, &BlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, BlurFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, BlurDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, BlurDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, BlurColourTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !BlurColourTex || !BlurDepthTex) { return; }
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::LoadEdge() {
	//mesh
	EdgeMesh = Mesh::GenerateQuad();
	//shader
	EdgeShader = new Shader(SHADERDIR"CourseworkProcessVertex.glsl",
		SHADERDIR"CourseworkSobelFragment.glsl");

	if (!EdgeShader->LinkProgram()) { return; }
	//generate textuer data
	glGenTextures(1, &EdgeDepthTex);
	glBindTexture(GL_TEXTURE_2D, EdgeDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &EdgeColourTex);
	glBindTexture(GL_TEXTURE_2D, EdgeColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//bind textureattachment
	glGenFramebuffers(1, &EdgeFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, EdgeFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, EdgeDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, EdgeDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, EdgeColourTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !EdgeDepthTex || !EdgeColourTex) { return; }
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::LoadBlurNew() {
	//mesh
	BlurNewMesh = Mesh::GenerateQuad();
	//shader
	BlurNewShader = new Shader(SHADERDIR"CourseworkProcessVertex.glsl",
		SHADERDIR"CourseworkBlurNewFragment.glsl");

	if (!BlurNewShader->LinkProgram()) { return; }
	//generate textuer data
	glGenTextures(1, &BlurNewDepthTex);
	glBindTexture(GL_TEXTURE_2D, BlurNewDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &BlurNewColourTex);
	glBindTexture(GL_TEXTURE_2D, BlurNewColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	//bind textureattachment
	glGenFramebuffers(1, &BlurNewFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, BlurNewFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, BlurNewDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, BlurNewDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, BlurColourTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !BlurNewColourTex || !BlurNewDepthTex) { return; }
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::LoadShadow() {

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	sceneShader = new Shader(SHADERDIR"CoureseworkSceneVertex.glsl", SHADERDIR"CoureseworkSceneFragment.glsl");
	shadowShader = new Shader(SHADERDIR"CoureseworkShadowVertex.glsl", SHADERDIR"CoureseworkShadowFragment.glsl");

	if (!sceneShader->LinkProgram() || !shadowShader->LinkProgram()) {
		return;
	}

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	floor = Mesh::GenerateQuad();
	floor->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	floor->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"brickDOT3.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

}

#pragma endregion

#pragma region DrawScene

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	if (changeScene == 1)
	{
		if (openProcess == 4)
		{
			DrawSkybox();
			DrawWater();
			//DrawNewWater();
			DrawHouse(houseNode);
			DrawPlanet(planetNode);
			DrawMaster();
			DrawHeightmap();
			DrawSphere(sphereNode);
			DrawParticle();
		}
		else if (openProcess == 5) {
			DrawEdge();
		}
		else if (openProcess == 6) {
			DrawBlur();
		}
		else if (openProcess == 7) {
			DrawBlurNew();
		}

		if (openMinimap == 1){
			DrawMiniMap();
		}

	}
	else {
		DrawSkyboxTwo();
		DrawShadow();
		DrawCombineShadow();
		DrawHeightmapTwo();
		DrawSphereTwo(sphereNodeTwo);
		DrawRocket(RocketNode);
	}
	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glUseProgram(skyboxShader->GetProgram());
	SetCurrentShader(skyboxShader);

	glDepthMask(GL_FALSE);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"starRatio"), starRatio);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTexStar"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapStar);

	textureMatrix = Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f)) *
		Matrix4::Rotation(-starRotation, Vector3(1.0f, 1.0f, 1.0f));

	UpdateShaderMatrices();

	cubeMesh->Draw();

	textureMatrix.ToIdentity();
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawWater() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(waterShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"starRatio"), starRatio);

	textureMatrixForStar = Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f)) *
		Matrix4::Rotation(-starRotation, Vector3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), 
		"textureMatrixForStar"), 1, false, (float*)&textureMatrixForStar);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTexStar"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapStar);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"dudvMap"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, waterDudv);

	modelMatrix =
		Matrix4::Translation(Vector3(2056, -466, 2056)) *
		Matrix4::Scale(Vector3(8000, 1, 8000)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(1.0f, 1.0f, 1.0f));

	UpdateShaderMatrices();
	waterMesh->Draw();

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	textureMatrixForStar.ToIdentity();
	glUseProgram(0);
}

void Renderer::DrawNewWater() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(NewWaterShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	Vector3 temp = Vector3(1, mapSpeed, 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "mapSpeed"),
		1, (float*)&temp);

	modelMatrix =
		Matrix4::Translation(Vector3(-1000, -475, -700)) *
		Matrix4::Scale(Vector3(1, 0.1, 1)) *
		Matrix4::Rotation(0, Vector3(0.0f, 0.0f, 0.0f));
	textureMatrix = Matrix4::Scale(Vector3(100.0f, 10.0f, 100.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(1.0f, 1.0f, 1.0f));

	UpdateShaderMatrices();
	NewWaterMesh->Draw();

	
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	glUseProgram(0);
}

void Renderer::DrawHouse(SceneNode* n) {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(houseShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());

	UpdateShaderMatrices();

	if (n->GetMesh()) {
		Matrix4 transform = Matrix4::Translation(Vector3(1000, -480, 1500))*
			n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(houseShader->GetProgram(),
			"modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(houseShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
	for (vector < SceneNode*>::const_iterator i = n->GetChildIteratorStart();i != n->GetChildIteratorEnd(); ++i) {
		DrawHouse(*i);
	}

	glUseProgram(0);
}

void Renderer::DrawMaster() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(masterShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glEnable(GL_CULL_FACE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),"cameraPos"), 1, (float*)&freeCamera->GetPosition());

	modelMatrix = 
		Matrix4::Translation(Vector3(1110, -460, 1475))*
		Matrix4::Scale(Vector3(1, 1, 1))*
		Matrix4::Rotation(160, Vector3(0.0f, 1.0f, .0f));

	UpdateShaderMatrices();
	masterNode -> Draw(*this);

	modelMatrix.ToIdentity();
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
}

void Renderer::DrawHeightmap() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(heightmapShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	Vector3 temp = Vector3(1, mapSpeed, 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "mapSpeed"),
		1, (float*)&temp);

	modelMatrix =
		Matrix4::Translation(Vector3(-800, -600, -700)) *
		Matrix4::Scale(Vector3(1, 1, 1)) *
		Matrix4::Rotation(0, Vector3(0.0f, 0.0f, .0f));

	UpdateShaderMatrices();
	heightMap->Draw();

	modelMatrix.ToIdentity();
	glUseProgram(0);
}

void Renderer::DrawParticle() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(showShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "snowTex"), 0);

	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		modelMatrix = Matrix4::Translation(ParticleSystem[i]->GetPosition())
			* viewMatrix.GetTransposedRotation()
			* Matrix4::Scale(ParticleSystem[i]->GetScale());
		UpdateShaderMatrices();
		ParticleSystem[i]->shape->SetTexture(snowTex);
		if (frameFrustum.InsideFrustum(ParticleSystem[i]->GetPosition(), 10.0f)){
			ParticleSystem[i]->DrawA();
		}
	}

	modelMatrix.ToIdentity();
	glUseProgram(0);
}

void Renderer::DrawMiniMap() {
	glUseProgram(houseShader->GetProgram());

	//get current picuture
	viewMatrix = miniMapCamera->BuildViewMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, miniMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);
		DrawSkybox();
		DrawWater();
		DrawHouse(houseNode);
		DrawPlanet(planetNode);
		DrawMaster();
		DrawHeightmap();
	//DrawParticle();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//draw processed picture
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(miniMapShader);

	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	modelMatrix = Matrix4::Scale(Vector3(0.5, 0.5, 0.5));
	UpdateShaderMatrices();

	miniMapMesh->SetTexture(miniMapColourTex);
	miniMapMesh->Draw();

	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = freeCamera->BuildViewMatrix();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void Renderer::DrawPlanet(SceneNode* n) {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(planetShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());

	UpdateShaderMatrices();

	//planetPositon = Vector3(positionChange.x * 20, positionChange.y * 20, positionChange.z * 20);
	if (n->GetMesh()) {
		Matrix4 transform = Matrix4::Translation(Vector3(positionChange.x * 10 + 1000, positionChange.y * 10 - 500, positionChange.z * 10 + 1500)) *
			n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale()) * Matrix4::Scale(Vector3(100, 100, 100))
			* Matrix4::Rotation(45, Vector3(1.0f, 0.0f, 0.0f));;
		glUniformMatrix4fv(glGetUniformLocation(planetShader->GetProgram(),
			"modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(planetShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
	for (vector < SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawPlanet(*i);
	}

	glUseProgram(0);
}

void Renderer::DrawSphere(SceneNode* n) {
	glUseProgram(sphereShader->GetProgram());
	SetCurrentShader(sphereShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"sphereMap"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sphereMap);

	UpdateShaderMatrices();
	if (n->GetMesh()) {
		Matrix4 transform = Matrix4::Translation(Vector3(0, -200, 0)) *
			n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale()) * Matrix4::Scale(Vector3(100, 100, 100))
			* Matrix4::Rotation(0, Vector3(0.0f, 0.0f, 0.0f));;
		glUniformMatrix4fv(glGetUniformLocation(sphereShader->GetProgram(),
			"modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(planetShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
	for (vector < SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawSphere(*i);
	}

	glUseProgram(0);
}

void Renderer::DrawBlur() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(BlurShader);
	//get current picuture

	glBindFramebuffer(GL_FRAMEBUFFER, BlurFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawSkybox();
	DrawWater();
	//DrawNewWater();
	DrawHouse(houseNode);
	DrawPlanet(planetNode);
	DrawMaster();
	DrawHeightmap();
	DrawSphere(sphereNode);
	DrawParticle();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//draw processed picture
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(BlurShader);
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), pixel_Size / width, pixel_Size / height);

	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	UpdateShaderMatrices();

	miniMapMesh->SetTexture(BlurColourTex);
	miniMapMesh->Draw();

	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = freeCamera->BuildViewMatrix();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void Renderer::DrawEdge() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(EdgeShader);
	//get current picuture

	glBindFramebuffer(GL_FRAMEBUFFER, EdgeFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawSkybox();
	DrawWater();
	//DrawNewWater();
	DrawHouse(houseNode);
	DrawPlanet(planetNode);
	DrawMaster();
	DrawHeightmap();
	DrawSphere(sphereNode);
	DrawParticle();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//draw processed picture
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(EdgeShader);
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), pixel_Size / width, pixel_Size / height);

	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	UpdateShaderMatrices();

	miniMapMesh->SetTexture(EdgeColourTex);
	miniMapMesh->Draw();

	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = freeCamera->BuildViewMatrix();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void Renderer::DrawBlurNew() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(BlurNewShader);
	//get current picuture

	glBindFramebuffer(GL_FRAMEBUFFER, BlurNewFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawSkybox();
	DrawWater();
	//DrawNewWater();
	DrawHouse(houseNode);
	DrawPlanet(planetNode);
	DrawMaster();
	DrawHeightmap();
	DrawSphere(sphereNode);
	DrawParticle();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//draw processed picture
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(BlurNewShader);
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), pixel_Size / width, pixel_Size / height);

	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	UpdateShaderMatrices();

	miniMapMesh->SetTexture(BlurColourTex);
	miniMapMesh->Draw();

	modelMatrix.ToIdentity();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = freeCamera->BuildViewMatrix();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

#pragma endregion

#pragma region UpdateScene

void Renderer::UpdateScene(float msec) {
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		changeScene = -changeScene;
		autoCamera->SetPitch(0);
		autoCamera->SetYaw(50);
		autoCamera->SetPosition(Vector3(1500, -300, 2000));
		freeCamera->SetPosition(autoCamera->GetPosition());
		freeCamera->SetPitch(autoCamera->GetPitch());
		freeCamera->SetYaw(autoCamera->GetYaw());
		changeCheck = 0.0f;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		changeCamera = -changeCamera;
	}	
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		openMinimap = -openMinimap;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
		openProcess = 4;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_5)) {
		openProcess = 5;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_6)) {
		openProcess = 6;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_7)) {
		openProcess = 7;
	}

	UpdateMyCamera(msec);

	UpdateLight();

	UpdateHeightmap();
	UpdateNewWater();
	UpdateParticle(msec);
	UpdateSkybox();

	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	masterNode->Update(msec);
	waterRotate += msec / ROTATESPEED;

	UpdateHeightmapTwo();
	hellNode->Update(msec);

}

void Renderer::UpdateMyCamera(float msec) {
	if (changeCamera == 1){
		freeCamera->UpdateCamera(msec);
	}
	else {
		freeCamera->SetPosition(autoCamera->GetPosition());
		freeCamera->SetPitch(autoCamera->GetPitch());
		freeCamera->SetYaw(autoCamera->GetYaw());
	}

	Vector3* tempVector =  new Vector3( freeCamera->GetPosition() - Vector3(0,-200,0));

	if (tempVector->Length() < 100 ) {
		freeCamera->SetPitch(0);
		freeCamera->SetYaw(50);
		freeCamera->SetPosition(Vector3(1500, -300, 2000));
		changeScene = -changeScene;
		changeCheck = 0.0f;
	}
	delete tempVector;
	viewMatrix = freeCamera->BuildViewMatrix();

	miniMapCamera->UpdateCamera(msec);
	miniMapCamera->SetPosition(Vector3(freeCamera->GetPosition().x + 700.0f, 3000, freeCamera->GetPosition().z + 100.0f));
	miniMapCamera->SetPitch(-90.0f);
	miniMapCamera->SetYaw(freeCamera->GetYaw() - 180);
	if (changeScene == 1){
		changeCheck += msec / 4000;
		autoCamera->SetPosition(Vector3(1000.0f + cos(changeCheck) * 500, -300.0f, 1500.0f + sin(changeCheck) * 500));
		autoCamera->SetYaw(freeCamera->GetYaw() - changeCheck / 60);
	}
	else {
		changeCheck += msec / 3000;
		autoCamera->SetPosition(Vector3(1000.0f + cos(changeCheck) * 1000, -300.0f, 1500.0f + sin(changeCheck) * 1000));
		autoCamera->SetYaw(freeCamera->GetYaw() - changeCheck / 30);
	}
	if (changeCheck >= 360.0f) {
		changeCheck -= 360.0f;
	}
}

void Renderer::UpdateNewWater() {
	newwaterTemp += newWaterAddDir;
	if (newwaterTemp >= 1.0f) newWaterAddDir = -NEWWATREMOVESPEED;
	if (newwaterTemp <= -1.0f) newWaterAddDir = NEWWATREMOVESPEED;
}

void Renderer::UpdateLight() {
	timeCos = timeCos + 0.01f;
	if (timeCos >= 360.0f) { timeCos -= 360.0f; }
	float sumCos = timeCos + 135.0f;
	float temp = 10000.0f * cos(timeCos);
	Vector3 tempPos = sunLight->GetPosition();
	if (changeScene == 1)
	{
		sunLight->SetPosition(Vector3(10000.0f * cos(timeCos), 10000.0f * sin(timeCos), (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)));
		positionChange = tempPos - sunLight->GetPosition();
		moonLight->SetPosition(Vector3(10000.0f * cos(sumCos), 10000.0f * sin(sumCos), moonLight->GetPosition().z));
	}
	else {
		sunLight->SetPosition(Vector3(100.0f * cos(timeCos), 100.0f * sin(timeCos), 500));
		positionChange = tempPos - sunLight->GetPosition();
		moonLight->SetPosition(Vector3(1000.0f * cos(sumCos), 1000.0f * sin(sumCos), moonLight->GetPosition().z));

	}
}

void Renderer::UpdateHeightmap() {
	mapSpeed += addDir;
	if (mapSpeed >= 1.0f) addDir = -MAP_CHANGE_SPEED;
	if (mapSpeed <= 0.0f) addDir = MAP_CHANGE_SPEED;
}

void Renderer::UpdateParticle(float msec) {
	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		ParticleSystem[i]->Update(msec);
	}
}

void Renderer::UpdateSkybox() {
	starRotation += 0.2f;
	if (starRotation >= 360.0f){starRotation -= 360.0f;}

	starRatio += addSpe;
	if (starRatio >= 0.9f) addSpe = -MAP_CHANGE_SPEED;
	if (starRatio <= 0.1f) addSpe = MAP_CHANGE_SPEED;
}

#pragma endregion

#pragma region OthersFunction

void Renderer::SetMoonLight(const Light& light) {
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"moonlightPos"), 1, (float*)&light.GetPosition());

	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
		"moonlightColour"), 1, (float*)&light.GetColour());

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"moonlightRadius"), light.GetRadius());
}

Renderer ::~Renderer(void) {

	delete sunLight;
	delete moonLight;

	delete freeCamera;

	delete cubeMesh;

	delete waterMesh;

	delete NewWaterMesh;

	delete houseNode;
	delete sceneMesh;


	delete masterNode;
	delete masterData;


	delete heightMap;

	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		delete ParticleSystem[i];
	}
	glDeleteTextures(1, &snowColourTex);

	delete miniMapMesh;
	glDeleteTextures(1, &miniMapColourTex);
	glDeleteTextures(1, &miniMapDepthTex);
	glDeleteFramebuffers(1, &miniMapFBO);

	delete planetNode;
	delete planetMesh;

	delete RocketNode;
	delete RocketMesh;

	delete sphereNode;
	delete sphereMesh;
}

#pragma endregion

#pragma region SceneTwo

void Renderer::LoadSkyboxTwo() {
	//mesh
	cubeMeshTwo = Mesh::GenerateQuad();

	//texture
	cubeMapTwo = SOIL_load_OGL_cubemap(
		TEXTUREDIR "5dim_ft.tga", TEXTUREDIR "5dim_bk.tga",
		TEXTUREDIR "5dim_up.tga", TEXTUREDIR "5dim_dn.tga",
		TEXTUREDIR "5dim_rt.tga", TEXTUREDIR "5dim_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);

	SetTextureRepeating(cubeMesh->GetTexture(), true);
	//shader
	skyboxShaderTwo = new Shader(SHADERDIR "CourseworkSkyboxVertexTwo.glsl",
		SHADERDIR "CourseworkSkyboxFragmentTwo.glsl");
	if (!skyboxShaderTwo->LinkProgram()) { return; }
	//update
}

void Renderer::DrawSkyboxTwo() {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(skyboxShaderTwo);

	glDepthMask(GL_FALSE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeMapTwo"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTwo);

	UpdateShaderMatrices();

	cubeMeshTwo->Draw();
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}


void Renderer::LoadHeightmapTwo() {
	//mesh
	heightMapTwo = new HeightMap(TEXTUREDIR"terrain.raw", 0);
	//texture
	heightMapTwo->SetTexture(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren Reds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMapTwo->SetBumpMap(SOIL_load_OGL_texture(
		TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!heightMapTwo->GetTexture() || !heightMapTwo->GetBumpMap()) { return; }
	SetTextureRepeating(heightMapTwo->GetTexture(), true);
	SetTextureRepeating(heightMapTwo->GetBumpMap(), true);
	//shader
	heightmapShaderTwo = new Shader(SHADERDIR "CourseworkHeightVertexTwo.glsl",
		SHADERDIR "CourseworkHeightFragmentTwo.glsl");
	if (!heightmapShaderTwo->LinkProgram()) { return; }
	//update
	changeSpeed = 0.0f;
	flowSpeed = 0.0f;
	addDirTwo = 0.1f;
}

void Renderer::DrawHeightmapTwo() {
	glUseProgram(heightmapShaderTwo->GetProgram());
	SetCurrentShader(heightmapShaderTwo);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"changeSpeed"), changeSpeed);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"flowSpeed"), flowSpeed);

	modelMatrix =
		Matrix4::Translation(Vector3(-2000, -600, -2000)) *
		Matrix4::Scale(Vector3(1, 1, 1)) *
		Matrix4::Rotation(0, Vector3(0.0f, 0.0f, .0f));
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	heightMapTwo->Draw();

	modelMatrix.ToIdentity();
	glUseProgram(0);
}

void Renderer::UpdateHeightmapTwo() {
	flowSpeed += addDirTwo;
	if (flowSpeed >= 1.0f) addDirTwo = -0.01f;
	if (flowSpeed <= 0.2f) addDirTwo = 0.01f;
	changeSpeed += 0.1f;
}


void Renderer::LoadSphereTwo(std::string OBJname) {
	//mesh &texture
	OBJMesh* tempMesh = new OBJMesh();
	tempMesh->LoadOBJMesh(OBJname);
	sphereMeshTwo = tempMesh;
	sphereNodeTwo = new SceneNode();
	SceneNode* temp = new SceneNode();
	temp->SetMesh(sphereMeshTwo);
	sphereNodeTwo->AddChild(temp);
	//texture
	sphereMapTwo = SOIL_load_OGL_cubemap(
		TEXTUREDIR "grave_ft.tga", TEXTUREDIR "grave_bk.tga",
		TEXTUREDIR "grave_up.tga", TEXTUREDIR "grave_dn.tga",
		TEXTUREDIR "grave_rt.tga", TEXTUREDIR "grave_lf.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	SetTextureRepeating(cubeMesh->GetTexture(), true);
	//shader
	sphereShaderTwo = new Shader(SHADERDIR"CourseworkSphereVertex.glsl",
		SHADERDIR"CourseworkSphereFragment.glsl");
	if (!sphereShaderTwo->LinkProgram()) {
		return;
	}
	//update
}

void Renderer::DrawSphereTwo(SceneNode* n) {
	glUseProgram(sphereShaderTwo->GetProgram());
	SetCurrentShader(sphereShaderTwo);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"sphereMap"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sphereMapTwo);

	UpdateShaderMatrices();
	if (n->GetMesh()) {
		Matrix4 transform = Matrix4::Translation(Vector3(0, -200, 0)) *
			n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale()) * Matrix4::Scale(Vector3(100, 100, 100))
			* Matrix4::Rotation(0, Vector3(0.0f, 0.0f, 0.0f));;
		glUniformMatrix4fv(glGetUniformLocation(sphereShaderTwo->GetProgram(),
			"modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(sphereShaderTwo->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
	for (vector < SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawSphereTwo(*i);
	}

	glUseProgram(0);
}


void Renderer::LoadRocket(std::string OBJname) {
	//mesh &texture
	OBJMesh* tempMesh = new OBJMesh();
	tempMesh->LoadOBJMesh(OBJname);
	RocketMesh = tempMesh;
	RocketNode = new SceneNode();
	SceneNode* temp = new SceneNode();
	temp->SetMesh(RocketMesh);
	RocketNode->AddChild(temp);
	//shader
	RocketShader = new Shader(SHADERDIR"CourseworkRockVertex.glsl",
		SHADERDIR"CourseworkRockFragment.glsl");
	if (!RocketShader->LinkProgram()) {
		return;
	}
	//update
	RocketPositon = Vector3(1000, 0, 1500);
}

void Renderer::DrawRocket(SceneNode* n) {
	glUseProgram(houseShader->GetProgram());
	SetCurrentShader(RocketShader);
	SetShaderLight(*sunLight);
	SetMoonLight(*moonLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&freeCamera->GetPosition());

	UpdateShaderMatrices();
	if (n->GetMesh()) {
		Matrix4 transform = Matrix4::Translation(Vector3(0, -600, 0)) *
			n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale()) * Matrix4::Scale(Vector3(100, 100, 100))
			* Matrix4::Rotation(45, Vector3(1.0f, 0.0f, 0.0f));;
		glUniformMatrix4fv(glGetUniformLocation(RocketShader->GetProgram(),
			"modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(RocketShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		n->Draw(*this);
	}
	for (vector < SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawRocket(*i);
	}

	glUseProgram(0);
}

void Renderer::DrawShadow() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(sunLight->GetPosition(), Vector3(0, 0, 0));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawCombineShadow() {
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&freeCamera->GetPosition());

	SetShaderLight(*sunLight);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = freeCamera->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();

	glUseProgram(0);
}

void Renderer::DrawMesh() {
	modelMatrix.ToIdentity();

	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	hellNode->Draw(*this);
}

void Renderer::DrawFloor() {
	modelMatrix = Matrix4::Rotation(90, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(450, 450, 1));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	floor->Draw();
}

#pragma endregion