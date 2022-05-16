
#ifdef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)
#endif // _DEBUG
#include "Rainbow3D.h"
#include <Windows.h>
#include <d3d11.h>
#include <iostream>
#include <memory>
#include <typeinfo>
using namespace Rainbow3D;

class A {
public:
    virtual ~A() {}
    virtual void Func() {
        _outputA("this is A.");
    }
};
class B : public A {
public:
    void Func() {
        _outputA("this is B.");
    }
};

namespace test {
    template <class _Ty>
    struct default_delete { // default deleter for unique_ptr
        constexpr default_delete() noexcept = default;

        template <class _Ty2, std::enable_if_t<std::is_convertible_v<_Ty2*, _Ty*>, int> = 0>
        default_delete(const default_delete<_Ty2>&) noexcept {}

        void operator()(_Ty* _Ptr) const noexcept /* strengthened */ { // delete a pointer
            static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");

            delete _Ptr;
        }
    };
}

void fff(int* p) { delete p; }

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto omColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);

    auto baseColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    auto baseList = CreateCommandList(device);
    auto texture = CreateTexture2DFromFile(device, L"C:\\Users\\WangYuzhi\\Desktop\\R-C.jpg");

    auto xxx = [](A* z) {delete z; };
    {
    }

    auto pp = std::is_convertible_v<A,B>;
    {
        std::unique_ptr<B, test::default_delete<B>> y(new B);
        std::unique_ptr<A, test::default_delete<A>> x(std::move(y));
        x.~unique_ptr();

        auto psda = x.get();


        UniquePtr<A> xx(new B);
       //std::is_
        UniquePtr<A> zz(UniquePtr<B>(new B));
    }
   

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