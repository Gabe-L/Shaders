#ifndef _TESSELATEDQUADMESH_H_
#define _TESSELATEDQUADMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class TesselatedQuadMesh : public BaseMesh
{
public:
	TesselatedQuadMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~TesselatedQuadMesh();

	virtual void sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

private:
	void initBuffers(ID3D11Device* device);

};

#endif