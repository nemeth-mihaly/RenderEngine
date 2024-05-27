#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <string>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/stb/stb_image.h"

////////////////////////////////////////////////////
//  class Texture_t
////////////////////////////////////////////////////

class Texture_t
{
 public:
    Texture_t();
    ~Texture_t();

    void LoadFromFile(const std::string& Filename);

    void Bind() const;

 private:
    GLuint Texture;
};