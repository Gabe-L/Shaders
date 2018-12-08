// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework

// Shader header files
#include "ExplosionShader.h"
#include "LightShader.h"
#include "TerrainShader.h"
#include "TextureShader.h"
#include "ShadowShader.h"
#include "BlurShader.h"
#include "DepthShader.h"
#include "DoFShader.h"

// Mesh header files
#include "TessellatedPlane.h"
#include "OrthoMesh.h"
#include "CubeMesh.h"
#include "SphereMesh.h"

// Misc header files
#include "Explosion.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

	void depthPass(XMMATRIX viewMatrix, XMMATRIX projectionMatrix, RenderTexture* texture_target);

	RenderTexture* FirstPass(RenderTexture* inputTexture);
	RenderTexture* HorizontalBlur(RenderTexture* inputTexture);
	RenderTexture* VerticalBlur(RenderTexture* inputTexture);
	RenderTexture* DoFPass(RenderTexture* inputTexture);

	void FinalPass(RenderTexture* inputTexture);

private:

	// Terrain
	TessellatedPlane* terrain;
	TerrainShader* terrainShader;
	int tessFactor;
	
	// Post processing
	OrthoMesh* orthoMesh;
	RenderTexture* targetTexture;
	TextureShader* textureShader;

	// Depth
	RenderTexture* cameraDepthTexture;
	DepthShader* depthShader;

	// Lighting
	ShadowShader* shadowShader;
	LightShader* lightShader;

	Light* spotLight;
	RenderTexture* spotLightDepth;

	// Test cube
	CubeMesh* cubeMesh;
	XMFLOAT3 lightPos;

	// Explosion
	Explosion* explosion;
	ExplosionShader* explosionShader;
	float timeTrack;

	// Depth of Field / Blur
	BlurShader* blurShader;
	DoFShader* dofShader;
	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	RenderTexture* cameraDepth;
	RenderTexture* DoFTexture;

	// Models
	Model* zepplin;
	XMFLOAT3 zepplinPos;
	
	Model* biplane;
	XMFLOAT3 biplanePos;

	// Testing purposes
	bool showGrass = true;
	int bladeCount = 3.0f;
	bool pauseExplosion = false;
	int terrainDimensions;
	RenderTexture* debugTexture;
	bool dofOn = true;
	bool shadowShowcase = false;
};

#endif