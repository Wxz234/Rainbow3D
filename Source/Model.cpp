#include "Model.h"

GLTFModel* CreateGLTFModelFromFile(const wchar_t* file) {
	
	return new GLTFModel();
}

void DestroyModel(Model* model) {
	delete model;
}
