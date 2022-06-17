#pragma once
#include "CorePreDef.h"

#include <d3d12.h>

namespace Rainbow3D {
	inline void CreateEmptyRootSignature(ID3D12Device* device, ID3D12RootSignature** ppRootSignature) {
		D3D12_ROOT_SIGNATURE_DESC desc{};
		//desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), __uuidof(**(ppRootSignature)), reinterpret_cast<void**>(ppRootSignature));
	}
}