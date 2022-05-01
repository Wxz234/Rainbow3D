#pragma once
#include "Render/Device/GraphicsDevice.h"
#include "Render/Renderer/Renderer.h"
#include "Runtime/Mesh/Mesh.h"
namespace Rainbow3D {
	class MeshRenderer : public Renderer {
	public:
		DISABLE_COPY_AND_ASSIGN(MeshRenderer)

		MeshRenderer(GraphcisDevice* device, Mesh* mesh);
		~MeshRenderer();
	private:
		GraphcisDevice* _device;
		Mesh* _mesh;

		class impl;
		impl* pimpl;
	};

	MeshRenderer* CreateMeshRenderer(GraphcisDevice* device, Mesh* mesh);
	
}