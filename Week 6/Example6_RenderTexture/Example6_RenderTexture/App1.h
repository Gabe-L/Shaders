// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "OnlyLightShader.h"
#include "EdgeShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void firstPass();
	void finalPass();
	void gui();

private:
	CubeMesh* cubeMesh;
	SphereMesh* sphereMesh;
	PlaneMesh* planeMesh;

	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMeshTwo;

	LightShader* lightShader;
	TextureShader* textureShader;
	OnlyLightShader* onlyLightShader;
	EdgeShader* edgeShader;

	Camera* rtCamera;

	RenderTexture* renderTexture;
	RenderTexture* renderTextureTwo;
	RenderTexture* edgeDetectionTexture;

	Light* light;
};

#endif