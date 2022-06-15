#pragma once
#include "CorePreDef.h"
#include <d3d12.h>
#include <wrl/client.h>

namespace Rainbow3D{
    class Device {
    public:
        Device(){
            D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, __uuidof(**(&m_device)), &m_device);
        }
    private:
        Microsoft::WRL::ComPtr<ID3D12Device6> m_device;
    };

    inline std::unique_ptr<Device> CreateDevice() {
        return std::unique_ptr<Device>(new Device);
    }
}