#include "Render/Graphics/GraphicsUtility.h"
namespace Rainbow3D {

	class DX11GraphicsUtility : public GraphicsUtility {
	public:

	};
	UniquePtr<GraphicsUtility> CreateGraphicsUtility(Device* device, SwapChain* swapchain) {
		return nullptr;
	}
}