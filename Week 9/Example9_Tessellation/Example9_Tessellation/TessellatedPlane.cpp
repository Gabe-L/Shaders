#include "TessellatedPlane.h"

// https://stackoverflow.com/questions/17805912/how-can-i-create-draw-render-gridplane-using-directx
// Practical rendering chapter nines

TessellatedPlane::TessellatedPlane(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int _scale, int _patchSize) : scale(_scale), patchSize(_patchSize)
{
	initBuffers(device);
}


TessellatedPlane::~TessellatedPlane()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}

void TessellatedPlane::initBuffers(ID3D11Device * device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	int numberOfPatches = scale * scale;

	vertexCount = numberOfPatches + (2 * scale) + 1;
	indexCount = 4 * numberOfPatches;

	VertexType_Texture* vertices = new VertexType_Texture[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	for (int y = 0; y < scale + 1; y++) {
		for (int x = 0; x < scale + 1; x++) {
			XMFLOAT2 patchPosition;
			int vertexIdentity = ((scale + 1) * y) + x;

			patchPosition.x = (-scale / 2) + (patchSize * x);
			patchPosition.y = (-scale / 2) + (patchSize * y);

			// Load the vertex array with data - setting the top left position of every patch
			vertices[vertexIdentity].position = XMFLOAT3(patchPosition.x, 0.0f, patchPosition.y);
			vertices[vertexIdentity].texture = XMFLOAT2((float)x / scale, (float)y / scale);

		}
	}

	int indexTrack = 0;

	for (int y = 0; y < scale; y++) {
		for (int x = 0; x < scale; x++) {
			indices[indexTrack + 0] = (y  + 0) + (x + 0) * (scale + 1);
			indices[indexTrack + 1] = (y  + 1) + (x + 0) * (scale + 1);
			indices[indexTrack + 2] = (y  + 1) + (x + 1) * (scale + 1);
			indices[indexTrack + 3] = (y  + 0) + (x + 1) * (scale + 1);

			indexTrack += 4;
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType_Texture) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

void TessellatedPlane::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType_Texture);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(top);
}