#include "LightShader.h"

LightShader::LightShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"light_vs.cso", L"light_ps.cso");
}


LightShader::~LightShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the light constant buffer.
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void LightShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC attenuationBufferDesc;

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
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	//creating control buffer
	attenuationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	attenuationBufferDesc.ByteWidth = sizeof(AttenuationBufferType);
	attenuationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	attenuationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	attenuationBufferDesc.MiscFlags = 0;
	attenuationBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&attenuationBufferDesc, NULL, &attenuationBuffer);


}

XMFLOAT4 float3_to_float4(XMFLOAT3 input, float w_val = 0.f) //converts a float3 to a float4 for padding
{
	return XMFLOAT4(input.x, input.y, input.z, w_val);
}

void LightShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture_two, Light* point_light[2], Light* dir_light[2])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//Additional
	// Send light data to pixel shader
	LightBufferType* lightPtr;
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;

	for (int i = 0; i < 2; i++) {
		//point lights
		lightPtr->ambient[i] = point_light[i]->getAmbientColour();
		lightPtr->diffuse[i] = point_light[i]->getDiffuseColour();
		lightPtr->position[i] = float3_to_float4(point_light[i]->getPosition());

		//directional lights
		lightPtr->ambient[i + 2] = dir_light[i]->getAmbientColour();
		lightPtr->diffuse[i + 2] = dir_light[i]->getDiffuseColour();
		lightPtr->direction[i] = float3_to_float4(dir_light[i]->getDirection());

	}

	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	AttenuationBufferType* attPtr;
	deviceContext->Map(attenuationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	attPtr = (AttenuationBufferType*)mappedResource.pData;
	attPtr->constant = 0.5f;
	attPtr->lin = 0.125f;
	attPtr->quadratic = 0.f;
	attPtr->padding = 0.f;

	deviceContext->Unmap(attenuationBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &attenuationBuffer);

	// Set resources for vertex shader
	deviceContext->VSSetShaderResources(0, 1, &texture_two);
	deviceContext->VSSetSamplers(0, 1, &sampleState);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);


}
