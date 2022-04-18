#include "Runtime/Model/Model.h"

namespace Rainbow3D {
	void DestroyModel(Model* model) {
		delete model;
	}
}