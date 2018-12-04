#include "DoFShader.h"



DoFShader::DoFShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"texture_vs.cso", L"DoF_ps.cso");
}


DoFShader::~DoFShader()
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

	if (dofBuffer)
	{
		dofBuffer->Release();
		dofBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void DoFShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC dofBufferDesc;

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

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	dofBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	dofBufferDesc.ByteWidth = sizeof(DoFBufferType);
	dofBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dofBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dofBufferDesc.MiscFlags = 0;
	dofBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&dofBufferDesc, NULL, &dofBuffer);

}


void DoFShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* blurTexture, ID3D11ShaderResourceView* depthTexture, float _dist, float _far, float _near, float _range)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Sned matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	DoFBufferType* dofPtr;
	deviceContext->Map(dofBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dofPtr = (DoFBufferType*)mappedResource.pData;
	dofPtr->Dist = _dist;
	dofPtr->Far = _far;
	dofPtr->Near = _near;
	dofPtr->Range = _range;
	deviceContext->Unmap(dofBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &dofBuffer);

	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &depthTexture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);
	deviceContext->PSSetShaderResources(2, 1, &blurTexture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}