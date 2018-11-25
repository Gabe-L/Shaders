#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class TerrainShader : public BaseShader
{
private:

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
	};

	struct TessellationBufferType
	{
		float tessellationFactor;
		XMFLOAT3 cameraPosition;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 direction;
		XMFLOAT4 position;
	};

	struct camBufferType
	{
		XMFLOAT4 camPos;
		float time;
		XMFLOAT3 padding;
	};

public:

	TerrainShader(ID3D11Device* device, HWND hwnd);
	~TerrainShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* grassTexture, ID3D11ShaderResourceView* heightTexture, ID3D11ShaderResourceView* mudTexture, ID3D11ShaderResourceView* lightDepth, float tesselationFactor, XMFLOAT3 cameraPosition, Light* light, float time);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* tessellationBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* camBuffer;
};

