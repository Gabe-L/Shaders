#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class ExplosionShader : public BaseShader
{
private:

	struct TimeBufferType
	{
		XMFLOAT4 displacementFactor;
	};

public:
	ExplosionShader(ID3D11Device* device, HWND hwnd);
	~ExplosionShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture_two, float time, float explodeOffset);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* timeBuffer;
};

