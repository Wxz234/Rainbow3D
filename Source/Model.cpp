#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Model.h"
#include "Log.h"

using json = nlohmann::json;

GLTFModel* CreateGLTFModelFromFile(const wchar_t* file) {
	std::ifstream ifs(file);
	json jf = json::parse(ifs);
	
	return new GLTFModel();
}

void DestroyModel(Model* model) {
	delete model;
}
