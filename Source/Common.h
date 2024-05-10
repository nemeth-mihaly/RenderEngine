#pragma once

#ifndef UNICODE
    #define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>

#include <3rdParty/KHR/khrplatform.h>

#include <3rdParty/glad/glad_wgl.h>
#include <3rdParty/glad/glad.h>

#include <3rdParty/glm/glm.hpp>
#include <3rdParty/glm/gtc/matrix_transform.hpp>
#include <3rdParty/glm/gtc/type_ptr.hpp>

#include <3rdParty/stb/stb_image.h>

constexpr GLuint InvalidGpuID = 0xffffffff;