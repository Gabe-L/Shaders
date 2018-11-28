// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "ExplosionShader.h"
#include "LightShader.h"
#include "TerrainShader.h"
#include "TextureShader.h"
#include "ShadowShader.h"
#include "BlurShader.h"
#include "DepthShader.h"
#include "DoFShader.h"
#include "TessellatedPlane.h"
#include "OrthoMesh.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
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
	XMFLOAT3 windPos;
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

	// Testing purposes
	int terrainDimensions;
	RenderTexture* debugTexture;
	PlaneMesh* testPlane;
	bool testRender = false;
};

#endif