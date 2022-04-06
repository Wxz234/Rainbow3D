#pragma once
#include "Device.h"
class Model {
public:
	virtual ~Model() {}
};

class GLTFModel :public Model {
public:
	GLTFModel() {}
	GLTFModel(const GLTFModel&) = default;
	GLTFModel(GLTFModel&&) noexcept = default;
	GLTFModel& operator=(const GLTFModel&) = default;
	GLTFModel& operator=(GLTFModel&&) noexcept = default;
private:
};

GLTFModel* CreateGLTFModelFromFile(const wchar_t* file);
void DestroyModel(Model* model);
