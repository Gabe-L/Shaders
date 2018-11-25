// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TessellatedPlane.h"
#include "TerrainShader.h"
#include "OrthoMesh.h"
#include "TextureShader.h"
#include "DepthShader.h"
#include "CubeMesh.h"
#include "ShadowShader.h"

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

	void depthPass(Light* lightUsed, RenderTexture* texture_target, bool ortho);

	void FirstPass();
	void ShadowPass();
	void FinalPass();

private:

	float timeTrack;

	// Terrain
	TessellatedPlane* terrain;
	TerrainShader* terrainShader;
	int tessFactor;
	
	// Post processing
	OrthoMesh* orthoMesh;
	RenderTexture* defaultView;
	TextureShader* textureShader;

	// Depth
	RenderTexture* cameraDepthTexture;
	DepthShader* depthShader;

	// Lighting
	Light* light;
	RenderTexture* lightDepth;
	ShadowShader* shadowShader;
	LightShader* lightShader;

	// Test cube
	CubeMesh* cubeMesh;

};

#endif