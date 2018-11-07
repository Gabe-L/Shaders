// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TessellationShader.h"
#include "TesselatedQuadMesh.h"

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
	TesselatedQuadMesh* mesh;
	TessellationShader* shader;

	int tessFactor;
	XMFLOAT4 wave_info;

};

#endif