#include "Json.h"

namespace Rainbow3D {
	std::ifstream& operator>>(std::ifstream& i, JsonObject& j) {
		return i;
	}
}
