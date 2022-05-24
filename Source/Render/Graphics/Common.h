#pragma once 

namespace Rainbow3D {
	enum RAINBOW_BIND_FLAG {
		BIND_VERTEX_BUFFER	= 0x1L,
        BIND_INDEX_BUFFER	= 0x2L,
        BIND_CONSTANT_BUFFER	= 0x4L,
        BIND_SHADER_RESOURCE	= 0x8L,
        BIND_STREAM_OUTPUT	= 0x10L,
        BIND_RENDER_TARGET	= 0x20L,
        BIND_DEPTH_STENCIL	= 0x40L,
        BIND_UNORDERED_ACCESS	= 0x80L,
	};

    enum class FORMAT {
        UNKNOWN
    };
}