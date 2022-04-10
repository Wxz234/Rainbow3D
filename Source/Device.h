#pragma once
#include "Window.h"

#include <Windows.h>
#include <d3d11.h>
#include <cstdint>

namespace Rainbow3D {
	struct Device {
		Device(HWND hwnd, uint32_t w, uint32_t h);
		~Device();
		Device(const Device&) = delete;
		Device(Device&&) noexcept = delete;
		Device& operator=(const Device&) = delete;
		Device& operator=(Device&&) noexcept = delete;

		void Present();
		void ClearRTV(const float ColorRGBA[4]);
		ID3D11Device* GetDevice() const noexcept;
		ID3D11DeviceContext* GetDeviceContext() const noexcept;
		void CreateTextureFromFile(const wchar_t* file, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
	private:
		class impl;
		impl* pimpl;
	};

	Device* CreateDevice(Window* window, uint32_t w, uint32_t h);
	void DestroyDevice(Device* device);

}