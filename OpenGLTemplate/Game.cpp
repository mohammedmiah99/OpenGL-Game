/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 4.0a 30/01/2016
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "Cube.h"
#include "Roof.h"
#include "NewShape.h"


// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pCar = NULL;
	m_pShip = NULL;
	m_pTreeMesh = NULL;
	m_pFenceMesh = NULL;
	m_pStreetLampMesh = NULL;
	m_pElephantMesh = NULL;
	m_pWolfMesh = NULL;
	m_pSphere = NULL;
	m_pCube = NULL;
	m_pRoof = NULL;
	m_pNewShape = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.1f;
	speed = 1;
	m_t = 0.0;
	m_cameraRotation = 0.0f;
	m_rotY = 0.0f;
	m_movement = 0;
	g = 0;
	horsey = false;
	fog = 0.0;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pCar;
	delete m_pShip;
	delete m_pTreeMesh;
	delete m_pFenceMesh;
	delete m_pStreetLampMesh;
	delete m_pElephantMesh;
	delete m_pWolfMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pCube;
	delete m_pNewShape;
	delete m_pRoof;
	delete m_pCatmullRom;
	delete m_pTimer;


	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pCar = new COpenAssetImportMesh;
	m_pShip = new COpenAssetImportMesh;
	m_pTreeMesh = new COpenAssetImportMesh;
	m_pFenceMesh = new COpenAssetImportMesh;
	m_pStreetLampMesh = new COpenAssetImportMesh;
	m_pElephantMesh = new COpenAssetImportMesh;
	m_pWolfMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pCube = new CCube;
	m_pNewShape = new CNewShape;
	m_pRoof = new CRoof;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pTimer = new CHighResolutionTimer;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("houseShader.vert");
	sShaderFileNames.push_back("houseShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	CShaderProgram *pShaderProgram = new CShaderProgram;
	pShaderProgram->CreateProgram();
	pShaderProgram->AddShaderToProgram(&shShaders[4]);
	pShaderProgram->AddShaderToProgram(&shShaders[5]);
	pShaderProgram->LinkProgram();
	m_pShaderPrograms->push_back(pShaderProgram);



	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "turf.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("corbel.ttf", 38);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pElephantMesh->Load("resources\\models\\Elephant\\elephant-lowpoly.obj");
	m_pWolfMesh->Load("resources\\models\\Wolf\\wolf.obj");
	m_pStreetLampMesh->Load("resources\\models\\StreetLamp\\StreetLamp.obj");
	m_pFenceMesh->Load("resources\\models\\Fence\\Fence.obj");
	m_pTreeMesh->Load("resources\\models\\Tree\\Tree.obj");
	m_pShip->Load("resources\\models\\Ship\\lowpolyship.obj");
	m_pCar->Load("resources\\models\\Car\\lowpolycar01.obj");

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Create a cube
	m_pCube->Create();

	//Create the octahedron shape
	m_pNewShape->Create("resources\\textures\\darkbrick.jpg");

	// Create the roof of the house
	m_pRoof->Create();


	// Initialise audio and play background music
	/*m_pAudio->Initialise();
	m_pAudio->LoadEventSound("Resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("Resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	m_pAudio->PlayMusicStream();
	*/

	//Call the CatmullRom methods
	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack();
	//Start a timer
	m_pTimer->Start();


}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	pMainProgram->SetUniform("CubeMapTex", 1);
	pMainProgram->SetUniform("sampler1", 2);
	pMainProgram->SetUniform("t", (float)m_pTimer->Elapsed()); // timer for fog pulse
	pMainProgram->SetUniform("fogpass", fog); //pass the fog choice - dense, no fog, pulse

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*

	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light

	glm::vec4 lightPosition2(0, 100, 0, 1);	//// Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light2.position", viewMatrix * lightPosition2); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light2.La", glm::vec3(0.2f, 0.0f, 0.7f));		// Ambient colour of light
	pMainProgram->SetUniform("light2.Ld", glm::vec3(0.2f, 0.0f, 0.7f));		// Diffuse colour of light
	pMainProgram->SetUniform("light2.Ls", glm::vec3(0.2f, 0.0f, 0.7f));		// Specular colour of light
	pMainProgram->SetUniform("light2.direction", glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
	pMainProgram->SetUniform("light2.exponent", 20.0f);
	pMainProgram->SetUniform("light2.cutoff", 30.0f);

	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property


	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render();
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	


	// Render the horse 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-100.0f, 0.0f, 0.0f));		//position
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);	// rotation
		modelViewMatrixStack.Scale(2.5f);	//increase in size
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHorseMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the lamp 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-100.0f, 5.0f, 25.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
	modelViewMatrixStack.Scale(2.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStreetLampMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Elephant 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(220.0f, 15.0f, -40.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 25.0f);
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pElephantMesh->Render();
	modelViewMatrixStack.Pop();

	//Render the Fences
	for (int x = 0; x < 600;x+=10) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-265+x, 2.5f, 165));
		modelViewMatrixStack.Scale(0.25f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pFenceMesh->Render();
		modelViewMatrixStack.Pop();
	}
	//Render the Fences
	for (int x = 0; x < 600; x += 50) { 
		for (int z = 0; z < 90; z += 10) {
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(-270+x, 2.5f, 170 + z));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 55.0f);
			modelViewMatrixStack.Scale(0.25f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pFenceMesh->Render();
			modelViewMatrixStack.Pop();
		}
	}
	//Render the Fences
	for (int x = 0; x < 600; x += 10) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-265 + x, 2.5f, -165));
		modelViewMatrixStack.Scale(0.25f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pFenceMesh->Render();
		modelViewMatrixStack.Pop();
	}
	//Render the Fences
	for (int x = 0; x < 600; x += 50) { 
		for (int z = 0; z < 90; z += 10) {
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(-270 + x, 2.5f, -250 + z));
			modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 55.0f);
			modelViewMatrixStack.Scale(0.25f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			m_pFenceMesh->Render();
			modelViewMatrixStack.Pop();
		}
	}
	//Render the Trees
	for (int z = 0; z < 500; z += 15) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-280, 10.0f, -250+z));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 55.0f);
		modelViewMatrixStack.Scale(5.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTreeMesh->Render();
		modelViewMatrixStack.Pop();
	}

	//Render the ship as the 3rd person mesh
	modelViewMatrixStack.Push();
	modelViewMatrixStack.ApplyMatrix(thirdPerson);
	modelViewMatrixStack.Translate(glm::vec3(0, 1.5, 0));
	modelViewMatrixStack.Scale(0.75f);
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 110.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pShip->Render();
	modelViewMatrixStack.Pop();

	// Render the Wolves 
	for (int i = 0; i < 60;) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(150.0f + i, 5.0f, 0.0f));
		modelViewMatrixStack.Scale(2.5f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pWolfMesh->Render();
		modelViewMatrixStack.Pop();
		i = i + 20;
	}
	// Render the octahedron
	for (int i = 0; i < 60;i+=20) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(250.0f, 20.0f+i, -100.0f + i));
		modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_rotY);
		modelViewMatrixStack.Scale(10.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pNewShape->Render();
		modelViewMatrixStack.Pop();
	}
	// Render the octahedron
	for (int i = 0; i < 80; i += 20) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(250.0f, 20.0f + i, 20.0f-i));
		modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), m_rotY);
		modelViewMatrixStack.Scale(10.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pNewShape->Render();
		modelViewMatrixStack.Pop();
	}
	
	// Render the barrel 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
		modelViewMatrixStack.Scale(5.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pBarrelMesh->Render();
	modelViewMatrixStack.Pop();

		// Render the sphere
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 150.0f));
		modelViewMatrixStack.Scale(2.0f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSphere->Render();
	modelViewMatrixStack.Pop();

	//Render the centreline
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
	m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderCentreline();
	modelViewMatrixStack.Pop();

	//Render the offset curves
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainProgram->SetUniform("bUseTexture", true); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
	m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderOffsetCurves();
	modelViewMatrixStack.Pop();

	//Render the track
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderTrack();
	modelViewMatrixStack.Pop();

	//Render the cars
	for (int z = -3; z <= 0; z++) {
		for (int x = -3; x <= 0; x++) {	
			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(x * 25, 5.0f, 80 +z *25));
			modelViewMatrixStack.Scale(4.0f);
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			pMainProgram->SetUniform("t", (float)m_pTimer->Elapsed());
			m_pCar->Render();
			modelViewMatrixStack.Pop();
		}
	}

	//Render the Roofs for the house
	for (int x = 0; x < 550; x += 50) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-250 + x, 37.0f, 250));
		modelViewMatrixStack.Scale(15.0f);
		pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", false);
		m_pRoof->Render();
		modelViewMatrixStack.Pop();
	}
	//Render the Roofs for the house
	for (int x = 0; x < 550; x += 50) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-250 + x, 37.0f, -250));
		modelViewMatrixStack.Scale(15.0f);
		pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", false);
		m_pRoof->Render();
		modelViewMatrixStack.Pop();
	}

	//Change the shader program to the house shader
	CShaderProgram *pShaderProgram = (*m_pShaderPrograms)[2];
	pShaderProgram->UseProgram();
	pShaderProgram->SetUniform("bUseTexture", true);
	pShaderProgram->SetUniform("sampler0", 0);
	pShaderProgram->SetUniform("sampler1", 2);
	// Set the projection matrix
	pShaderProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	//Render the cubes for the house
	for (int x = 0; x < 550; x += 50) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-250 + x, 14.5f, 250));
		modelViewMatrixStack.Scale(15.0f);
		pShaderProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pShaderProgram->SetUniform("t", (float)m_pTimer->Elapsed());	//timer for fog pulse
		pShaderProgram->SetUniform("fogpass", fog);		//pass the fog choice - dense, no fog, pulse
		m_pCube->Render();
		modelViewMatrixStack.Pop();

	}
	//Render the cubes for the house
	for (int x = 0; x < 550; x += 50) {
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-250 + x, 14.5f, -250));
		modelViewMatrixStack.Scale(15.0f);
		pShaderProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pShaderProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pShaderProgram->SetUniform("t", (float)m_pTimer->Elapsed());	//timer for fog pulse
		pShaderProgram->SetUniform("fogpass", fog);		//pass the fog choice - dense, no fog, pulse
		m_pCube->Render();
		modelViewMatrixStack.Pop();

	}

	// Draw the 2D graphics after the 3D graphics
	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());	

}


// Update method runs repeatedly with the Render method
void Game::Update()
{
	m_rotY += 0.001f*m_dt;	//rotation variable
	m_t += (float)(0.01f * m_dt);

	m_pAudio->Update();

	//To increase/decrease the speed of the player
	if (GetKeyState('2') & 0x80 && speed <15 && speed>-16) {
		m_cameraSpeed += 0.01f;
		speed += 1;
	}
	if (GetKeyState('3') & 0x80 && speed <16 && speed >-15) {
		m_cameraSpeed -= 0.01f;
		speed -= 1;
	}

	m_currentDistance += m_dt * m_cameraSpeed;
	glm::vec3 hel;
	glm::vec3 hell;

	glm::vec3 tenOffset = glm::vec3(10, 10, 10);

	glm::vec3 pNext;
	glm::vec3 t;
	glm::vec3 p;
	glm::vec3 g = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 1, 0);

	//if right button or 'D' is pressed, increase movement and leftRight variable
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		leftRight += 0.01f*m_dt;
		if (m_movement < 50) {
			m_movement++;
		}
	}
	//if left button or 'A' is pressed, decrease movement and leftRight variable
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80){
		leftRight -= 0.01f*m_dt;
		if (m_movement > -50) {
			m_movement--;
		}
	}
	//if Up button or 'W' is pressed, increase rotation variable
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)
		m_cameraRotation += 0.001f*m_dt;
	//if Down button or 'S' is pressed, decrease rotation variable
	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
		m_cameraRotation -= 0.001f*m_dt;


	m_pCatmullRom->Sample(m_currentDistance, p, g);

	m_pCatmullRom->Sample((m_currentDistance + 1.0f), pNext, g);
	t = pNext - p;
	hel = tenOffset * t;
	hell = p + hel;

	//Create an offset to camera
	glm::vec3 Offset1 = glm::vec3(0, 3, 0);
	glm::vec3 Offset2 = glm::vec3(20, 20, 20);

	glm::vec3 off = glm::normalize(glm::cross(t, glm::vec3(0, 1, 0)));
	leftRight = glm::clamp(leftRight, -5.0f, 5.0f);
	glm::vec3 offsetv = glm::vec3(off.x*leftRight, off.y*leftRight, off.z*leftRight);

	glm::vec3 ss = p + Offset1 + offsetv;
	glm::vec3 sss = p + Offset2*t;
	glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), m_cameraRotation, t);

	//Create third person camera pos
	m_pCatmullRom->Sample(m_currentDistance + 6, pos, g);
	glm::vec3 n = glm::normalize(glm::cross(t, g));
	glm::vec3 b = glm::normalize(glm::cross(n, t));

	pos2 = glm::mat4(glm::mat3(t, b, n));
	thirdPerson = glm::mat4();
	glm::vec3 thr = glm::vec3(0, 20, 0);

	//if 'X' is pressed, enable third person
	if ((GetKeyState('X') & 0x80)||horsey == true) {
		horsey = true;
		thirdPerson = glm::translate(pos + offsetv);
		thirdPerson *= pos2;
	}
	//if 'Z' is pressed, disable third person
	if (GetKeyState('Z') & 0x80) {
		horsey = false;
	}
	//if 'E' is pressed, pass value 0.0 to the shader
	if (GetKeyState('E') & 0x80) {
		fog = 0.0;
	}	
	//if 'R' is pressed, pass value 1.0 to the shader
	if (GetKeyState('R') & 0x80) {
		fog = 1.0;
	}
	//if 'T' is pressed, pass value 2.0 to the shader
	if (GetKeyState('T') & 0x80) {
		fog = 2.0;
	}
	//Set the camera
	m_pCamera->Set(ss, sss, up);
	m_pCamera->Update(m_dt);

}

void Game::DisplayFrameRate()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;


	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }
	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
	}
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		m_pFtFont->Render(100, height - 20, 20, "Movement: %d", m_movement);	//Render movement variable

		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 50, 20, "Speed: %d", speed);	//Render speed variable
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
