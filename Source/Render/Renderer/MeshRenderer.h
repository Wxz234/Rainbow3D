#pragma once
#include "Render/Renderer/Renderer.h"
#include "Runtime/Mesh/Mesh.h"
namespace Rainbow3D {
	class MeshRenderer : public Renderer {
	public:
		DISABLE_COPY_AND_ASSIGN(MeshRenderer)

		MeshRenderer(GraphcisDevice* device, Mesh* mesh) :Renderer(device), _mesh(mesh) {}
	private:
		Mesh* _mesh;
	};
}