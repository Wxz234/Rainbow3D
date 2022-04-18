#include "Runtime/Object/Object.h"
namespace Rainbow3D {
	void DestroyRObject(RObject* obj) {
		delete obj;
	}
}