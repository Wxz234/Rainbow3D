#pragma once
#include "CorePreDef.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace Rainbow3D{
    class Device {
    public:
        Device(){
#ifdef _DEBUG
            Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
            D3D12GetDebugInterface(__uuidof(**(&debugController)), &debugController);
            debugController->EnableDebugLayer();
#endif // _DEBUG
            D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, __uuidof(**(&m_device)), &m_device);
            D3D12_COMMAND_QUEUE_DESC desc{};
            desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            m_device->CreateCommandQueue(&desc, __uuidof(**(&m_main_queue)), &m_main_queue);
        }

        ID3D12CommandQueue* GetMainQueue() const {
            return m_main_queue.Get();
        }

        void WaitSwapChain(IDXGISwapChain* swapchain) {
            Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapchain;
            swapchain->QueryInterface(__uuidof(**(&_swapchain)), &_swapchain);

        }

    private:

        Microsoft::WRL::ComPtr<ID3D12Device6> m_device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_main_queue;
    };

    inline std::unique_ptr<Device> CreateDevice() {
        return std::unique_ptr<Device>(new Device);
    }
}