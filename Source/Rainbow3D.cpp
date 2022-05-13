#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <memory>
using namespace Rainbow3D;

class A {
public:
    virtual ~A() {}
};
class B : public A {
public:

};


template <class _Ty>
struct pzz { // default deleter for unique_ptr
    constexpr pzz() noexcept = default;

    template <class _Ty2, std::enable_if_t<std::is_convertible_v<_Ty2*, _Ty*>, int> = 0>
    pzz(const pzz<_Ty2>&) noexcept {}

    void operator()(_Ty* _Ptr) const noexcept /* strengthened */ { // delete a pointer
        static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
        delete _Ptr;
    }
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto omColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);

    auto baseColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    auto baseList = CreateCommandList(device);
    auto texture = CreateTexture2DFromFile(device, L"C:\\Users\\WangYuzhi\\Desktop\\R-C.jpg");
 
    window->Show();
    while (window->IsActive()) {
        window->Dispatch();
        baseList->Open();
        float outputColor[4] = { 1.f,0.2f,0.5f,1.f };
        baseList->ClearRTV(omColor, outputColor);
        baseList->Close();
        device->ExecuteCommandList(baseList);
        device->Present(texture);
    }
    DestroyRainbowObject(texture);
    DestroyRainbowObject(baseList);
    DestroyRainbowObject(baseColor);
    DestroyRainbowObject(omColor);
    DestroyRainbowObject(device);
    DestroyRainbowObject(window);

    return 0;
}