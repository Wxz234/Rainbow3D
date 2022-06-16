#include "Rainbow3D.h"

using namespace Rainbow3D;
void Draw(Device* device, SwapChain* swapchain, Microsoft::WRL::ComPtr<ID3D12CommandAllocator>* pAllocator, ID3D12GraphicsCommandList* list) {
    auto index = swapchain->GetIndex();
    pAllocator[index]->Reset();
    list->Reset(pAllocator[index].Get(), nullptr);
    list->Close();
    device->Execute(list);
    swapchain->Present(0);
    device->WaitSwapChain(swapchain->GetSwapChain());
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    constexpr uint32 w = 800, h = 600;
    auto window = CreateRenderWindow(L"RainbowEditor", w, h);
    auto device = CreateDevice();
    auto swapchain = CreateSwapChain(device.get(), window.get(), w, h);

    auto device_ptr = device->GetDevice();
    //device_ptr->CreateCommandAllocator()
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mainRenderAllocator[FRAME_NUM];
    for (uint32 i = 0;i < FRAME_NUM; ++i) {
        device_ptr->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(&mainRenderAllocator[i]), &mainRenderAllocator[i]);
    }
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mainRenderlist;
    Microsoft::WRL::ComPtr<ID3D12Device4> device4;
    device_ptr->QueryInterface(device4.ReleaseAndGetAddressOf());
    device4->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(&mainRenderlist), &mainRenderlist);
    window->Show();
    return window->RunLoop(Draw, device.get(), swapchain.get(), mainRenderAllocator, mainRenderlist.Get());
}