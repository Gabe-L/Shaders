#pragma once
#include "SphereMesh.h"
#include "ExplosionShader.h"
#include "DepthShader.h"
#include <vector>

class Explosion
{
public:
	Explosion(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, HWND hwnd, ExplosionShader* _explosionShader, DepthShader* _depthShader);
	~Explosion();

	void Update(float deltaTime);
	void Render(XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* composeTexture, ID3D11ShaderResourceView* burnTexture, float sceneTimer);
	void RenderDepth(XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	XMMATRIX GenerateView(int index);
	std::vector<ID3D11ShaderResourceView*> getDepthResources();

	Light* getLight() { return explosionPointLight; }
	RenderTexture* getShadowMap(int index) { return shadowMaps[index]; }
	XMFLOAT3 worldPosition;
	RenderTexture* shadowMaps[6];

private:
	int shadowMapWidth, shadowMapHeight;

	SphereMesh* explosionSphere;
	ExplosionShader* explosionShader;
	DepthShader* depthShader;
	XMFLOAT3 startDiffuse;
	float explosionTimer;
	Light* explosionPointLight;
	XMFLOAT3 directions[6];
	std::vector<ID3D11ShaderResourceView*> depthResources;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

};

