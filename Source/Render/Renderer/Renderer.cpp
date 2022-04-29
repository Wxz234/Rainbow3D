#include "Render/Renderer/Renderer.h"
namespace Rainbow3D {
	void DestroyRenderer(Renderer* renderer) {
		delete renderer;
	}
}