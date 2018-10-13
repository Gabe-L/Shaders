#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class SpecularShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float specularPower;
		XMFLOAT4 specularColour;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
		XMFLOAT4 testVars;
	};

public:

	SpecularShader(ID3D11Device* device, HWND hwnd);
	~SpecularShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, XMFLOAT3 camPos, XMFLOAT4 testVals);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11Buffer * cameraBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};