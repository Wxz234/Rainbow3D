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
            D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, __uuidof(**(&m_device)), &m_device);
            D3D12_COMMAND_QUEUE_DESC desc{};
            desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            m_device->CreateCommandQueue(&desc, __uuidof(**(&m_main_queue)), &m_main_queue);
            // swapchain
            m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(**(&m_fence)), &m_fence);
            m_fenceValues[0]++;
            m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        }

        ID3D12CommandQueue* GetMainQueue() const {
            return m_main_queue.Get();
        }
        ID3D12Device* GetDevice() const {
            return m_device.Get();
        }

        void Execute(ID3D12CommandList *list) {
            m_main_queue->ExecuteCommandLists(1, &list);
        }

        void WaitSwapChain(IDXGISwapChain* swapchain) {
            Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapchain;
            swapchain->QueryInterface(__uuidof(**(&_swapchain)), &_swapchain);

            const uint64 currentFenceValue = m_fenceValues[m_frameIndex];
            m_main_queue->Signal(m_fence.Get(), currentFenceValue);

            m_frameIndex = _swapchain->GetCurrentBackBufferIndex();

            if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex])
            {
                m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent);
                WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
            }

            m_fenceValues[m_frameIndex] = currentFenceValue + 1;
        }
        ~Device() {
            m_main_queue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]);
            m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent);
            WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
            m_fenceValues[m_frameIndex]++;
            OutputDebugStringA("123\n");
            CloseHandle(m_fenceEvent);
        }
    private:
        uint32 m_frameIndex = 0;
        HANDLE m_fenceEvent;
        Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
        uint64 m_fenceValues[FRAME_NUM]{};

        Microsoft::WRL::ComPtr<ID3D12Device6> m_device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_main_queue;
    };

    inline std::unique_ptr<Device> CreateDevice() {
        return std::unique_ptr<Device>(new Device);
    }
}