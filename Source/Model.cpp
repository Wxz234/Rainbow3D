#include "Model.h"

GLTFModel* CreateGLTFModelFromFile(const wchar_t* file) {
	return new GLTFModel();
}

void DestroyGLTFModel(GLTFModel* model) {
	delete model;
}
