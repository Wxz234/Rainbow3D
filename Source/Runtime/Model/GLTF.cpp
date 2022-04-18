#include "Runtime/Model/GLTF.h"
#include "Core/Log/Log.h"
#include "Core/Json/Json.h"

#include <fstream>

namespace Rainbow3D {
    bool utf8_check_is_valid(const char* string,int length)
    {
        int c, i, ix, n, j;
        for (i = 0, ix = length; i < ix; i++)
        {
            c = (unsigned char)string[i];
            //if (c==0x09 || c==0x0a || c==0x0d || (0x20 <= c && c <= 0x7e) ) n = 0; // is_printable_ascii
            if (0x00 <= c && c <= 0x7f) n = 0; // 0bbbbbbb
            else if ((c & 0xE0) == 0xC0) n = 1; // 110bbbbb
            else if (c == 0xed && i < (ix - 1) && ((unsigned char)string[i + 1] & 0xa0) == 0xa0) return false; //U+d800 to U+dfff
            else if ((c & 0xF0) == 0xE0) n = 2; // 1110bbbb
            else if ((c & 0xF8) == 0xF0) n = 3; // 11110bbb
            //else if (($c & 0xFC) == 0xF8) n=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
            //else if (($c & 0xFE) == 0xFC) n=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
            else return false;
            for (j = 0; j < n && i < ix; j++) { // n bytes matching 10bbbbbb follow ?
                if ((++i == ix) || (((unsigned char)string[i] & 0xC0) != 0x80))
                    return false;
            }
        }
        return true;
    }

	Model* CreateGLTFModelFromFile(const wchar_t* file) {
		std::ifstream ifs(file, std::ifstream::binary);
		if (!ifs.is_open()) {
			Rainbow3D_Error("Can't open the file.");
			return nullptr;
		}
		ifs.seekg(0, ifs.end);
		int filesize = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		if (filesize < 0) {
			ifs.close();
			Rainbow3D_Error("Can't get file size.");
			return nullptr;
		}
		char* buffer = new char[filesize];
		ifs.read(buffer, filesize);
		ifs.close();

        if (!utf8_check_is_valid(buffer, filesize)) {
            ifs.close();
            delete[]buffer;
            Rainbow3D_Error("File format is not UTF8.");
            return nullptr;
        }

        ifs.close();
        delete[]buffer;
		return new GLTFModel;
	}
}