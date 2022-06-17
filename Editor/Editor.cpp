#include "Rainbow3D.h"
#include "Shader/PostProcessVS.h"
#include "Shader/PostProcessPS.h"
#include "d3dx12.h"
using namespace Rainbow3D;
void Draw(Device* device, SwapChain* swapchain, Microsoft::WRL::ComPtr<ID3D12CommandAllocator>* pAllocator, ID3D12GraphicsCommandList* list, ID3D12RootSignature*root, ID3D12PipelineState *pso) {
    auto index = swapchain->GetIndex();
    pAllocator[index]->Reset();
    list->Reset(pAllocator[index].Get(), pso);
    
    list->SetGraphicsRootSignature(root);
    CD3DX12_VIEWPORT m_viewport(0.0f, 0.0f, 800.f, 600.f);
    list->RSSetViewports(1, &m_viewport);
    CD3DX12_RECT m_scissorRect(0, 0, 800, 600);
    list->RSSetScissorRects(1, &m_scissorRect);

    Microsoft::WRL::ComPtr<ID3D12Resource> buffer;
    auto swapchain_ptr = swapchain->GetSwapChain();
    swapchain_ptr->GetBuffer(index, __uuidof(&buffer), &buffer);
    // Indicate that the back buffer will be used as a render target.
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    list->ResourceBarrier(1, &barrier);

    auto rtvHandle = swapchain->GetRTV(index);
    list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    list->DrawInstanced(3, 1, 0, 0);

    // Indicate that the back buffer will now be used to present.
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(buffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    list->ResourceBarrier(1, &barrier);

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
        { "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, 2 };
    psoDesc.pRootSignature = rootsignature.Get();
    psoDesc.VS = { PostProcessVS,sizeof(PostProcessVS) };
    psoDesc.PS = { PostProcessPS,sizeof(PostProcessPS) };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline;
    device4->CreateGraphicsPipelineState(&psoDesc, __uuidof(&pipeline), &pipeline);

    window->Show();
    window->RunLoop(Draw, device.get(), swapchain.get(), mainRenderAllocator, mainRenderlist.Get(), rootsignature.Get(), pipeline.Get());
    device.reset();
    return 0;
}