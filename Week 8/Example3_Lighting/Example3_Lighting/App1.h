// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "textureShader.h"
#include "boxBlurShader.h"

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

	void FirstPass();

	void boxBlur();

	void FinalPass();

private:
	LightShader* shader;
	SphereMesh* mesh;
	Light* light;

	RenderTexture* renderTexture;
	RenderTexture* boxBlurTexture;

	OrthoMesh* orthoMesh;
	TextureShader* textureShader;
	boxBlurShader* boxShader;

	XMFLOAT2 resolution;

};

#endif