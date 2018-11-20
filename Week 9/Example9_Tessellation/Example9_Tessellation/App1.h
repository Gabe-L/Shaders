// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TessellationShader.h"
//#include "TesselatedQuadMesh.h"
#include "TessellatedPlane.h"
#include "GrassShader.h"

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
	//TessellationMesh* mesh;
	//TesselatedQuadMesh* mesh;
	TessellatedPlane* mesh;
	PlaneMesh* plane;

	TessellationShader* shader;
	
	Light* testLight;
	GrassShader* grassShader;

	XMFLOAT3 lightPos;

	int tessFactor;
	XMFLOAT4 wave_info;

};

#endif