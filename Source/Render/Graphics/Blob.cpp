#include "Render/Graphics/Blob.h"
#include <d3dcommon.h>
#include <wrl.h>
namespace Rainbow3D {

	class DX11BLOB : public Blob {
	private:

		virtual void* GetBufferPointer() const noexcept {
			return m_blob->GetBufferPointer();
		}
		virtual std::size_t GetBufferSize() {
			return m_blob->GetBufferSize();
		}

		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
				
	};


}