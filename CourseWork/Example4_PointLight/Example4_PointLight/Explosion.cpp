#include "Explosion.h"



Explosion::Explosion(int _terrainDimensions, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, HWND hwnd, ExplosionShader* _explosionShader, DepthShader* _depthShader) : terrainDimensions(_terrainDimensions), explosionTimer(0.0f), device(_device), deviceContext(_deviceContext), explosionShader(_explosionShader), depthShader(_depthShader), shadowMapWidth(4096), shadowMapHeight(4096)
{
	explosionSphere = new SphereMesh(device, deviceContext, 100);

	for (int i = 0; i < 6; i++) {
		shadowMaps[i] = new RenderTexture(device, shadowMapWidth, shadowMapHeight, 0.1f, 100.f);
	}

	startDiffuse = XMFLOAT3(1.0f, 0.5f, 0.0f);

	worldPosition = XMFLOAT3(55.0f, 6.0f, 55.0f);

	explosionPointLight = new Light;
	explosionPointLight->setAmbientColour(0.1f, 0.1f, 0.1f, 1.0f);
	explosionPointLight->setDiffuseColour(startDiffuse.x, startDiffuse.y, startDiffuse.z, 1.0f);
	explosionPointLight->setPosition(worldPosition.x, worldPosition.y, worldPosition.z);
	explosionPointLight->generateProjectionMatrix(0.1f, 100.f);
	explosionPointLight->generateViewMatrix();

	directions[0] = XMFLOAT3(0.0f, 1.0f, 0.0f);	// Up;
	directions[1] = XMFLOAT3(0.0f, -1.0f, 0.0f);// Down
	directions[2] = XMFLOAT3(1.0f, 0.0f, 0.0f);	// Right
	directions[3] = XMFLOAT3(-1.0f, 0.0f, 0.0f);// Left
	directions[4] = XMFLOAT3(0.0f, 0.0f, 1.0f);	// Fowards
	directions[5] = XMFLOAT3(0.0f, 0.0f, -1.0f);// Backwards

	depthResources.resize(6);
}


Explosion::~Explosion()
{
	if (explosionSphere) {
		delete explosionSphere;
		explosionSphere = 0;
	}

	for (int i = 0; i < 6; i++) {
		if (shadowMaps[i]) {
			delete shadowMaps[i];
			shadowMaps[i] = 0;
		}
	}

	if (explosionPointLight) {
		delete explosionPointLight;
		explosionPointLight = 0;
	}

}

void Explosion::Update(float deltaTime)
{
	explosionPointLight->setPosition(worldPosition.x, worldPosition.y, worldPosition.z);

	explosionTimer += deltaTime;

	if (explosionTimer > 3.0f) {
		explosionTimer = 0.0f;

		float xRand = 10 + (std::rand() % ((terrainDimensions - 10) - 10 + 1));
		float zRand = 10 + (std::rand() % ((terrainDimensions - 10) - 10 + 1));
		float yRand = 20 + (std::rand() % (30 - 20 + 1));


		worldPosition = XMFLOAT3(xRand, yRand, zRand);
	}
}

void Explosion::Render(XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* composeTexture, ID3D11ShaderResourceView* burnTexture, float sceneTimer)
{
	XMMATRIX worldMatrix = XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z);

	XMFLOAT3 lightFade;
	if (explosionTimer < 1.0f) {
		// Fade in
		lightFade = XMFLOAT3(
			startDiffuse.x * min(explosionTimer, 1.0f),
			startDiffuse.y * min(explosionTimer, 1.0f),
			startDiffuse.z * min(explosionTimer, 1.0f));
	}
	else {
		// Fade out
		lightFade = XMFLOAT3(
			startDiffuse.x * min((3.0f - explosionTimer) / 2.0f, 1.0f),
			startDiffuse.y * min((3.0f - explosionTimer) / 2.0f, 1.0f),
			startDiffuse.z * min((3.0f - explosionTimer) / 2.0f, 1.0f));
	}

	explosionPointLight->setDiffuseColour(lightFade.x, lightFade.y, lightFade.z, 1.0f);

	explosionSphere->sendData(deviceContext);
	explosionShader->setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, composeTexture, burnTexture, sceneTimer, explosionTimer);
	explosionShader->render(deviceContext, explosionSphere->getIndexCount());
}

void Explosion::RenderDepth(XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (explosionTimer < 2.0f) {
		XMMATRIX worldMatrix = XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z);
		explosionSphere->sendData(deviceContext);
		depthShader->setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
		depthShader->render(deviceContext, explosionSphere->getIndexCount());
	}
}

XMMATRIX Explosion::GenerateView(int index)
{
	explosionPointLight->setDirection(directions[index].x, directions[index].y, directions[index].z);

	if (index < 2) {
		float dirY = explosionPointLight->getDirection().y;
		dirY = (dirY > 0) ? 1 : -1;

		XMFLOAT3 lightDirectionFloat3 = XMFLOAT3(0.0f, dirY, 0.0f);
		XMFLOAT3 lightUpFloat3 = XMFLOAT3(0.0f, 0.0f, 1.0f);

		XMVECTOR lightPos = XMLoadFloat3(&explosionPointLight->getPosition());
		XMVECTOR lightDirection = XMLoadFloat3(&lightDirectionFloat3);
		XMVECTOR lightUp = XMLoadFloat3(&lightUpFloat3);

		return XMMatrixLookToLH(lightPos, lightDirection, lightUp);
	}
	else {
		explosionPointLight->generateViewMatrix();
		return explosionPointLight->getViewMatrix();
	}
}

std::vector<ID3D11ShaderResourceView*> Explosion::getDepthResources()
{
	for (int i = 0; i < 6; i++) {
		depthResources[i] = shadowMaps[i]->getShaderResourceView();
	}

	return depthResources;
}
