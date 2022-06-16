#pragma once
#include "CorePreDef.h"
#include "Platform/Window.h"
#include "Graphics/Device.h"
#include <d3d12.h>
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
            _desc.BufferCount = FRAME_NUM;
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

            Microsoft::WRL::ComPtr<ID3D12Device> m_device;
            queue->GetDevice(__uuidof(**(&m_device)), &m_device);
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = FRAME_NUM;
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            m_device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(**(&m_rtvHeap)), &m_rtvHeap);
            m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
            for (uint32 n = 0; n < FRAME_NUM; ++n)
            {
                Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets;
                m_swapchain->GetBuffer(n, __uuidof(**(&m_renderTargets)), &m_renderTargets);
                m_device->CreateRenderTargetView(m_renderTargets.Get(), nullptr, rtvHandle);
                rtvHandle.ptr = rtvHandle.ptr + m_rtvDescriptorSize;
            }
        }

        void Present(uint32 sync) {
            m_swapchain->Present(sync, 0);
        }

        IDXGISwapChain* GetSwapChain() const {
            return m_swapchain.Get();
        }

        D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(uint32 n) const {
            auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
            rtvHandle.ptr = rtvHandle.ptr + n * m_rtvDescriptorSize;
            return rtvHandle;
        }
    private:
        Microsoft::WRL::ComPtr<IDXGIFactory7> m_factory;
        Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapchain;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        uint32 m_rtvDescriptorSize;
    };

    inline std::unique_ptr<SwapChain> CreateSwapChain(Device* device, RWindow* window, uint32 w, uint32 h) {
        return std::unique_ptr<SwapChain>(new SwapChain(device->GetMainQueue(), window->GetHWND(), w, h));
    }
}