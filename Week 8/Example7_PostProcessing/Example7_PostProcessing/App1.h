// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "VerticalBlurShader.h"
#include "HorizontalBlurShader.h"

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
	void downSample();
	void verticalBlur();
	void horizontalBlur();
	void upSample();
	void finalPass();
	void gui();

private:

	float downSampleX, downSampleY;

	CubeMesh* cubeMesh;
	OrthoMesh* orthoMesh;
	OrthoMesh* smallOrthoMesh;

	LightShader* lightShader;
	TextureShader* textureShader;

	RenderTexture* downSampleTexture;
	RenderTexture* upSampleTexture;
	RenderTexture* renderTexture;
	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	VerticalBlurShader* verticalBlurShader;
	HorizontalBlurShader* horizontalBlurShader;
	
	Light* light;
};

#endif