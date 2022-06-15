#pragma once
#include "Core/CorePreDef.h"
#include "Runtime/Component/Component.h"
namespace Rainbow3D {
	class GameObject {
	public:
		GameObject(const std::string& name) : m_name(name) {}
		virtual ~GameObject() {}
	protected:
		std::string m_name;
	};
}