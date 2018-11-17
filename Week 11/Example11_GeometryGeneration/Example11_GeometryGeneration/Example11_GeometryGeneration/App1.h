// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "QuadPointMesh.h"
#include "PlaneMesh.h"
#include "TextureShader.h"

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

private:
	GeometryShader* geometryShader;
	TextureShader* textureShader;
	//PointMesh* mesh;
	PlaneMesh* plane;
	QuadPointMesh* mesh;
	Light* testLight;

	float timeTrack;

};

#endif