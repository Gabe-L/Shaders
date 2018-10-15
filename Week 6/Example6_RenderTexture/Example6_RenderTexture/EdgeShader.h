#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class EdgeShader : public BaseShader
{

public:
	EdgeShader(ID3D11Device* device, HWND hwnd);
	~EdgeShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);

private:
	void initShader(WCHAR*, WCHAR*);

};

