#include <cstdlib>
#define JSON_TRY_USER if(true)
#define JSON_CATCH_USER(exception) if(false)
#define JSON_THROW_USER(exception) { std::abort();}

#include "Runtime/Model/Model.h"
#include "Runtime/Model/GLTF.h"
#include "Core/Log/Log.h"
#include "Core/JSON/json.hpp"

#include <fstream>

namespace Rainbow3D {

    Model* CreateModelFromFile(const wchar_t* file) {

        std::ifstream i(file);
        if (!i.is_open()) {
            Rainbow3D_Error("Can't open the file.");
            return nullptr;
        }

        nlohmann::json j;
        i >> j;


        return new GLTFModel;
    }
}
