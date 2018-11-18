// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TextureShader.h"
#include "VerticalBlurShader.h"
#include "HorizontalBlurShader.h"
#include "MouseShader.h"
#include "DepthShader.h"
#include "DoFShader.h"

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
	void mouseHighlight();
	void upSample();
	void dofPass();
	void finalPass();
	void gui();

	void depthPass(Camera* cam, RenderTexture* texture_target);

private:

	float downSampleX, downSampleY;
	XMFLOAT2 resolution;
	float blurDirection;
	float blurDist;
	float horScale, verScale;

	CubeMesh* cubeMesh;
	OrthoMesh* orthoMesh;
	OrthoMesh* smallOrthoMesh;

	LightShader* lightShader;
	TextureShader* textureShader;
	VerticalBlurShader* verticalBlurShader;
	HorizontalBlurShader* horizontalBlurShader;
	MouseShader* mouseShader;
	DepthShader* depthShader;
	DoFShader* dofShader;

	RenderTexture* downSampleTexture;
	RenderTexture* upSampleTexture;
	RenderTexture* mouseTexture;
	RenderTexture* renderTexture;
	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	RenderTexture* camDepth;
	RenderTexture* dofTexture;

	Light* light;
};

#endif