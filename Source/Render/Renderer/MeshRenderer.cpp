#include "Render/Renderer/MeshRenderer.h"
#include <d3d11.h>
#include <wrl/client.h>
namespace Rainbow3D {
	class MeshRenderer::impl {
	public:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	};
	MeshRenderer::MeshRenderer(GraphcisDevice* device, Mesh* mesh) {
		_device = device; 
		_mesh = mesh;
		pimpl = new impl;
		//ID3D11Device* pDevice = reinterpret_cast<ID3D11Device*>(device->GetNativeDevice());
		//ID3D11DeviceContext* pContext = reinterpret_cast<ID3D11DeviceContext*>(device->GetNativeDeviceContext());

		//D3D11_BUFFER_DESC vbd = {};
		//vbd.Usage = D3D11_USAGE_IMMUTABLE;
		//vbd.ByteWidth = sizeof(Vector3) * _mesh->vertices.size();
		//vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//vbd.CPUAccessFlags = 0;
		////// 新建顶点缓冲区
		//D3D11_SUBRESOURCE_DATA InitData ={};
		//InitData.pSysMem = _mesh->vertices.data();
		//pDevice->CreateBuffer(&vbd, &InitData, pimpl->m_pVertexBuffer.GetAddressOf());
	}

	MeshRenderer::~MeshRenderer() {
		delete pimpl;
	}

	MeshRenderer* CreateMeshRenderer(GraphcisDevice* device, Mesh* mesh) {
		return new MeshRenderer(device, mesh);
	}
}