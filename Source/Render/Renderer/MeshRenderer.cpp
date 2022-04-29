#include "Render/Renderer/MeshRenderer.h"
namespace Rainbow3D {
	MeshRenderer* CreateMeshRenderer(GraphcisDevice* device, Mesh* mesh) {
		return new MeshRenderer(device, mesh);
	}
}