#pragma once
#include "Core/CoreTypes.h"
#include "Core/CoreDef.h"
#include "Platform/Window/Window.h"

namespace Rainbow3D {

	class GraphicsObject {
	public:
		virtual ~GraphicsObject() {}
	};

	class GraphicsList : public GraphicsObject {
	public:
		virtual void Close() = 0;
	};

	class GraphicsDevice : public GraphicsObject {
	public:
		virtual void Present() = 0;
		virtual void ClearRTV(const float ColorRGBA[4]) = 0;
		virtual void ExecuteCommandList(GraphicsList* list) = 0;
	};

	GraphicsDevice* CreateGraphicsDevice(WindowContext* context, uint32 width, uint32 height);
	void DestroyGraphicsDevice(GraphicsDevice* device);

	GraphicsList* CreateGraphicsList(GraphicsDevice* device);
	void DestroyGraphicsList(GraphicsList* list);
}