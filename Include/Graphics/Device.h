#pragma once
#include "CorePreDef.h"
#include <d3d12.h>
#include <wrl/client.h>

namespace Rainbow3D{
    class Device {
    public:
        Device(){}
    private:
        Microsoft::WRL::ComPtr<ID3D12Device6> m_device;
    };
}