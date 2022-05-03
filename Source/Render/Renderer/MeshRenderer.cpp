#include "Render/Renderer/MeshRenderer.h"
#include <d3d11.h>
#include <wrl/client.h>
namespace Rainbow3D {
	class MeshRenderer::impl {
	public:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	};
	MeshRenderer::MeshRenderer(GraphicsDevice* device, Mesh* mesh) {
		_device = device; 
		_mesh = mesh;
		pimpl = new impl;
	}

	MeshRenderer::~MeshRenderer() {
		delete pimpl;
	}

	MeshRenderer* CreateMeshRenderer(GraphicsDevice* device, Mesh* mesh) {
		return new MeshRenderer(device, mesh);
	}
}