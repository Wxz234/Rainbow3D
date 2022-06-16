#include "Rainbow3D.h"
#include "Shader/PostProcessVS.h"
#include "d3dx12.h"
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

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
    CreateEmptyRootSignature(device4.Get(), &rootsignature);

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = rootsignature.Get();
    psoDesc.VS = {};
    psoDesc.PS = {};
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    window->Show();
    return window->RunLoop(Draw, device.get(), swapchain.get(), mainRenderAllocator, mainRenderlist.Get());
}