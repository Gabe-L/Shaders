#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class DoFShader : public BaseShader
{
public:
	DoFShader(ID3D11Device* device, HWND hwnd);
	~DoFShader();

	struct DoFBufferType
	{
		float Near;
		float Far;
		float Range;
		float Dist;
	};

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* blurTexture, ID3D11ShaderResourceView* depthTexture, float _dist, float _far, float _near, float _range);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* dofBuffer;
};