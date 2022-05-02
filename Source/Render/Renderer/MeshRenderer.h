#pragma once
#include "Render/Device/GraphicsDevice.h"
#include "Render/Renderer/Renderer.h"
#include "Runtime/Mesh/Mesh.h"
namespace Rainbow3D {
	class MeshRenderer : public Renderer {
	public:
		DISABLE_COPY_AND_ASSIGN(MeshRenderer)

		MeshRenderer(GraphicsDevice* device, Mesh* mesh);
		~MeshRenderer();
	private:
		GraphicsDevice* _device;
		Mesh* _mesh;

		class impl;
		impl* pimpl;
	};

	MeshRenderer* CreateMeshRenderer(GraphicsDevice* device, Mesh* mesh);
	
}