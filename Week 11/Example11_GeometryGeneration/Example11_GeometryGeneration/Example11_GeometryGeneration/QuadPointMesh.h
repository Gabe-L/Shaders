#ifndef _QUADPOINTMESH_H_
#define _QUADPOINTMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class QuadPointMesh : public BaseMesh
{
public:
	QuadPointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~QuadPointMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

private:
	void initBuffers(ID3D11Device* device);

};

#endif