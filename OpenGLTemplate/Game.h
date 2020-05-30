#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CCube;
class CRoof;
class CNewShape;
class CToonSphere;
class CHighResolutionTimer;


class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh *m_pTreeMesh;
	COpenAssetImportMesh *m_pFenceMesh;
	COpenAssetImportMesh *m_pElephantMesh;
	COpenAssetImportMesh *m_pWolfMesh;
	COpenAssetImportMesh *m_pStreetLampMesh;
	COpenAssetImportMesh *m_pCar;
	COpenAssetImportMesh *m_pShip;
	CSphere *m_pSphere;
	CCube *m_pCube;
	CRoof *m_pRoof;
	CNewShape *m_pNewShape;
	CToonSphere *m_pToonSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom *m_pCatmullRom;
	CHighResolutionTimer *m_pTimer;


	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	float m_t;
	float g;

	// third person variables
	glm::vec3 pos;
	glm::mat4 pos2;
	glm::mat4 thirdPerson;



public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	float m_currentDistance;
	float m_cameraRotation;

	float m_cameraSpeed;
	float leftRight = 0; 	//lateral postion on track
	float m_rotY; // Clockwise or anticlockwise rotation of camera
	int m_movement;	//lateral postion on track for HUD
	bool horsey; //First or thrid person
	float fog;	//Fog - dense, pulse, no fog
	int speed; // Speed variable - increase or decrease


};
