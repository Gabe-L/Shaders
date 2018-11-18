#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class DoFShader : public BaseShader
{
public:
	DoFShader(ID3D11Device* device, HWND hwnd);
	~DoFShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* blurTexture, ID3D11ShaderResourceView* depthTexture);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
};