#pragma once
#include <Windows.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <cstdint>
#include "Window.h"
namespace Rainbow3D {
	struct Device {
		Device(HWND hwnd, uint32_t w, uint32_t h);
		Device(const Device&) = delete;
		Device(Device&&) noexcept = delete;
		Device& operator=(const Device&) = delete;
		Device& operator=(Device&&) noexcept = delete;

		void Present() {
			m_swapChain->Present(1, 0);
		}

		void ClearRTV(const float ColorRGBA[4]) {
			m_DeviceContext->ClearRenderTargetView(m_rtv.Get(), ColorRGBA);
		}

		ID3D11Device *GetDevice() const noexcept {
			return m_Device.Get();
		}

		ID3D11DeviceContext* GetDeviceContext() const noexcept {
			return m_DeviceContext.Get();
		}

		void CreateTextureFromFile(const wchar_t* file);
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_rt;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
	};

	Device* CreateDevice(Window* window, uint32_t w, uint32_t h);
	void DestroyDevice(Device* device);

}