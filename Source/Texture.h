#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>
#include <string>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/stb/stb_image.h"

////////////////////////////////////////////////////
//  class Texture
////////////////////////////////////////////////////

class Texture
{
public:
    Texture(uint32_t target);
    ~Texture();

    void LoadFromFile(const std::string& resource);

    void BindUnit(uint32_t unit = 0);

private:
    uint32_t    m_TextureID;
};

typedef std::shared_ptr<Texture> StrongTexturePtr;