#include <fstream>
bool isXXX() {
	std::ifstream i("bindless_vs.cso");
	if (i.is_open()) {
		return true;
	}return false;
}
int main() {
	return 0;
}