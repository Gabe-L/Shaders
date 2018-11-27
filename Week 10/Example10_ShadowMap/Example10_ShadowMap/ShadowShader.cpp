// texture shader.cpp
#include "shadowshader.h"


ShadowShader::ShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"shadow_vs.cso", L"shadow_ps.cso");
}


ShadowShader::~ShadowShader()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (lightBuffer)
	{	
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void ShadowShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Sampler for shadow map sampling.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&samplerDesc, &sampleStateShadow);

	// Setup light buffer
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

}

XMMATRIX ShadowShader::YAxisViewMatrix(XMFLOAT3 direction, XMFLOAT3 position)
{
	float dirY = direction.y;
	dirY = (dirY > 0) ? 1 : -1;

	XMFLOAT3 lightDirectionFloat3 = XMFLOAT3(0.0f, dirY, 0.0f);
	XMFLOAT3 lightUpFloat3 = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMVECTOR lightPos = XMLoadFloat3(&position);
	XMVECTOR lightDirection = XMLoadFloat3(&lightDirectionFloat3);
	XMVECTOR lightUp = XMLoadFloat3(&lightUpFloat3);

	return XMMatrixLookToLH(lightPos, lightDirection, lightUp);
}


void ShadowShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView*depthMap2, ID3D11ShaderResourceView*depthArray[6], Light* light, Light* light2, Light* pointLight)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	
	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	XMMATRIX tLightViewMatrix = XMMatrixTranspose(light->getViewMatrix());
	XMMATRIX tLightProjectionMatrix = XMMatrixTranspose(light->getProjectionMatrix());

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	
	// Light view one
	dataPtr->lightView = tLightViewMatrix;
	dataPtr->lightProjection = tLightProjectionMatrix;

	XMMATRIX tLightViewMatrix2 = XMMatrixTranspose(light2->getViewMatrix());
	XMMATRIX tLightProjectionMatrix2 = XMMatrixTranspose(light2->getProjectionMatrix());


	// Light view two
	dataPtr->lightView2 = tLightViewMatrix2;
	dataPtr->lightProjection2 = tLightProjectionMatrix2;

	XMFLOAT3 directions[6] = {
		XMFLOAT3(0.0f ,1, 0.0f),	// Up
		XMFLOAT3(0.0f , -1.0f, 0.0f),	// Down
		XMFLOAT3(1,0.0f,0),	// Right
		XMFLOAT3(-1,0.0f,0),	// Left
		XMFLOAT3(0,0.0f,1),	// Fowards
		XMFLOAT3(0,0.0f,-1)	// Backwards
	};

	for (int i = 0; i < 6; i++) {

		pointLight->setDirection(directions[i].x, directions[i].y, directions[i].z);
		XMMATRIX viewMatrix;

		if (pointLight->getDirection().x == 0 && pointLight->getDirection().z == 0) {
			viewMatrix = YAxisViewMatrix(pointLight->getDirection(), pointLight->getPosition());
		}
		else {
			pointLight->generateViewMatrix();
			viewMatrix = pointLight->getViewMatrix();
		}

		XMMATRIX tLightViewMatrixPoint = XMMatrixTranspose(viewMatrix);
		XMMATRIX tLightProjectionMatrixPoint = XMMatrixTranspose(pointLight->getProjectionMatrix());


		// Light view two
		dataPtr->lightViews[i] = tLightViewMatrixPoint;
		dataPtr->lightProjections[i] = tLightProjectionMatrixPoint;
	}
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	
	// Light one
	lightPtr->ambient[0] = light->getAmbientColour();
	lightPtr->diffuse[0] = light->getDiffuseColour();

	XMFLOAT4 lightDir;
	lightDir.x = light->getDirection().x;
	lightDir.y = light->getDirection().y;
	lightDir.z = light->getDirection().z;
	lightDir.w = 1.0f;

	lightPtr->direction[0] = lightDir;

	// Light two
	lightPtr->ambient[1] = light2->getAmbientColour();
	lightPtr->diffuse[1] = light2->getDiffuseColour();

	lightDir.x = light2->getDirection().x;
	lightDir.y = light2->getDirection().y;
	lightDir.z = light2->getDirection().z;
	lightDir.w = 1.0f;

	lightPtr->direction[1] = lightDir;

	// Point light
	lightPtr->ambient[2] = pointLight->getAmbientColour();
	lightPtr->diffuse[2] = pointLight->getDiffuseColour();

	lightDir.x = pointLight->getPosition().x;
	lightDir.y = pointLight->getPosition().y;
	lightDir.z = pointLight->getPosition().z;
	lightDir.w = 1.0f;

	lightPtr->direction[2] = lightDir;

	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &depthMap);
	deviceContext->PSSetShaderResources(2, 1, &depthMap2);
	deviceContext->PSSetShaderResources(3, 6, depthArray);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);
}