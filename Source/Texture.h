#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>
#include <string>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/stb/stb_image.h"

//-----------------------------------------------------------------------------
// class Texture
//-----------------------------------------------------------------------------

class Texture
{
public:
    Texture(uint32_t target, uint32_t wrapping, uint32_t filtering);
    ~Texture();

    void Load(const std::string& filename);
    void Load(const std::string& filename, uint8_t*& _out_pData);

    void BindUnit(uint32_t unit = 0);

    uint32_t GetTextureID() { return m_textureID; }

private:
    uint32_t    m_textureID;
};