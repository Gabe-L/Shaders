#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class TextureShaderBlend : public BaseShader
{
public:
	TextureShaderBlend(ID3D11Device* device, HWND hwnd);
	~TextureShaderBlend();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture_one, ID3D11ShaderResourceView* texture_two);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;

};
