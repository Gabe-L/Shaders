#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class MiniMapShader : public BaseShader
{
private:
	struct CameraBufferType
	{
		XMFLOAT4 cameraPosition;
		XMFLOAT2 resolution;
		XMFLOAT2 padding;
	};

public:

	MiniMapShader(ID3D11Device* device, HWND hwnd);
	~MiniMapShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, XMFLOAT3 camPos, XMFLOAT2 resolution);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer * cameraBuffer;
};