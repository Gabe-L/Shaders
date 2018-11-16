// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"
#include "SphereMesh.h"
#include "CubeMesh.h"
#include "OrthoMesh.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void depthPass(Light* light_used, RenderTexture* texture_target, int matrix_type);
	void finalPass();
	void gui();

private:
	TextureShader* textureShader;
	PlaneMesh* mesh;
	CubeMesh* cube;
	SphereMesh* sphere;
	SphereMesh* lightSphere;

	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh2;

	Light* light;
	Light* light2;

	Model* model;
	ShadowShader* shadowShader;
	DepthShader* depthShader;

	RenderTexture* shadowMap;
	RenderTexture* shadowMap2;

	float rotationTrack;
	XMFLOAT3 lightPos;
	XMFLOAT3 lightDir;

};

#endif