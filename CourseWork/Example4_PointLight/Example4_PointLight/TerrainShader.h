#pragma once


#include "DXF.h"
#include "Explosion.h"

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
		XMMATRIX spotLightProjection;
		XMMATRIX spotLightView;
		XMMATRIX explosionLightProjections;
		XMMATRIX explosionLightViews[6];
	};

	struct TessellationBufferType
	{
		float tessellationFactor;
		XMFLOAT3 cameraPosition;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 direction[2];
		XMFLOAT4 position[2];
	};

	struct camBufferType
	{
		XMFLOAT4 camPos;
		float time;
		float showGrass;
		XMFLOAT2 padding;
	};

public:

	TerrainShader(ID3D11Device* device, HWND hwnd);
	~TerrainShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* grassTexture, ID3D11ShaderResourceView* heightTexture, ID3D11ShaderResourceView* mudTexture, Explosion* explosion, float tesselationFactor, XMFLOAT3 cameraPosition, float _showGrass, float time, Light* spotLight, ID3D11ShaderResourceView* spotLightDepth);

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

