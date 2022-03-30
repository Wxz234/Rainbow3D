#pragma once
#include "Decive.h"

class GLTFModel  {
public:
	GLTFModel() {}
	GLTFModel(const GLTFModel&) = default;
	GLTFModel(GLTFModel&&) noexcept = default;
	GLTFModel& operator=(const GLTFModel&) = default;
	GLTFModel& operator=(GLTFModel&&) noexcept = default;
private:
};

GLTFModel* CreateGLTFModelFromFile(const wchar_t* file);
void DestroyGLTFModel(GLTFModel* model);
