#pragma once
#include "CorePreDef.h"
#include "Platform/Window.h"
#include "Graphics/Device.h"
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace Rainbow3D {
    class SwapChain {
    public:
        SwapChain(ID3D12CommandQueue* queue, HWND hwnd, uint32 w, uint32 h) {
            UINT32 flag = 0;
#ifdef _DEBUG
            flag = DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG
            //D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, __uuidof(**(&m_device)), &m_device);
            CreateDXGIFactory2(flag, __uuidof(**(&m_factory)), &m_factory);

            DXGI_SWAP_CHAIN_DESC1 _desc = {};
            _desc.BufferCount = 3;
            _desc.Width = w;
            _desc.Height = h;
            _desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            _desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            _desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            _desc.SampleDesc.Count = 1;
            _desc.SampleDesc.Quality = 0;
            _desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            _desc.Scaling = DXGI_SCALING_STRETCH;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
            fsSwapChainDesc.Windowed = TRUE;

            m_factory->CreateSwapChainForHwnd(queue, hwnd, &_desc, &fsSwapChainDesc, nullptr, &m_swapchain);
            m_factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
        }

        void Present(uint32 sync) {
            m_swapchain->Present(sync, 0);
        }

        IDXGISwapChain* GetSwapChain() const {
            return m_swapchain.Get();
        }
    private:
        Microsoft::WRL::ComPtr<IDXGIFactory7> m_factory;
        Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapchain;
    };

    inline std::unique_ptr<SwapChain> CreateSwapChain(Device* device, RWindow* window, uint32 w, uint32 h) {
        return std::unique_ptr<SwapChain>(new SwapChain(device->GetMainQueue(), window->GetHWND(), w, h));
    }
}