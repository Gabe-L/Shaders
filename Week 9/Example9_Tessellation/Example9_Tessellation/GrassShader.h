// Light shader.h
// Geometry shader example.
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class GrassShader : public BaseShader
{

public:

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 position;
	};

	struct camBufferType
	{
		XMFLOAT4 camPos;
		float time;
		XMFLOAT3 padding;
	};

	GrassShader(ID3D11Device* device, HWND hwnd);
	~GrassShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, XMFLOAT3 camPos, float time);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* camBuffer;

};
