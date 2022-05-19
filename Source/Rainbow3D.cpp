
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
void fff(A* p) { }

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

        //using pointer = _Ty*;
    };

    struct Foo { // object to manage
        Foo() { std::cout << "Foo ctor\n"; }
        Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
        Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
        ~Foo() { std::cout << "~Foo dtor\n"; }
    };

    struct D { // deleter
        D() {};
        D(const D&) { std::cout << "D copy ctor\n"; }
        D(D&) { std::cout << "D non-const copy ctor\n"; }
        D(D&&) { std::cout << "D move ctor \n"; }
        void operator()(Foo* p) const {
            std::cout << "D is deleting a Foo\n";
            delete p;
        };
    };
}


void f(int *p) {
    delete p;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    uint32 width = 800, height = 600;
    auto window = CreateRenderWindow(L"Rainbow3D", width, height);
    auto device = CreateGraphicsDevice(window->GetContext(), width, height);
    auto omColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);

    auto baseColor = CreateRenderTarget(device, width, height, FORMAT::RGBA8_UNORM);
    auto baseList = CreateCommandList(device);
    auto texture = CreateTexture2DFromFile(device, L"C:\\Users\\WangYuzhi\\Desktop\\R-C.jpg");
    auto b_d = [](test::D* b) {delete b; };
    auto int_d = [](int* a) {delete a; };


    
    {
        using namespace test;
        std::cout << "Example constructor(1)...\n";
        UniquePtr<Foo> up1;  // up1 为空
        UniquePtr<Foo> up1b(nullptr);  // up1b 为空

        std::cout << "Example constructor(2)...\n";
        {
            UniquePtr<Foo> up2(new Foo); // up2 现在占有 Foo
        } // Foo 被删除

        std::cout << "Example constructor(3)...\n";
        D d;
        {  // 删除器类型不是引用
            UniquePtr<Foo, D> up3(new Foo, d); // 复制删除器
        }
        {  // 删除器类型是引用 
            UniquePtr<Foo, D&> up3b(new Foo, d); // up3b 保有到 d 的引用
        }

        std::cout << "Example constructor(4)...\n";
        {  // 删除器不是引用
            UniquePtr<Foo, D> up4(new Foo, D()); // 移动删除器
        }

        std::cout << "Example constructor(5)...\n";
        {
            UniquePtr<Foo> up5a(new Foo);
            UniquePtr<Foo> up5b(std::move(up5a)); // 所有权转移
        }

        std::cout << "Example constructor(6)...\n";
        {
            UniquePtr<Foo, D> up6a(new Foo, d); // 复制 D
            UniquePtr<Foo, D> up6b(std::move(up6a)); // 移动 D

            UniquePtr<Foo, D&> up6c(new Foo, d); // D 是引用
            UniquePtr<Foo, D> up6d(std::move(up6c)); // 复制 D
        }
    }

    std::cin;
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