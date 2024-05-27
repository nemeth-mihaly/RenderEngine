#include "Texture.h"

////////////////////////////////////////////////////
//  Texture_t Implementation
////////////////////////////////////////////////////

Texture_t::Texture_t()
{
}

Texture_t::~Texture_t()
{
    glDeleteTextures(1, &Texture);
}

void Texture_t::LoadFromFile(const std::string& Filename)
{
    stbi_set_flip_vertically_on_load(true);

    glCreateTextures(GL_TEXTURE_2D, 1, &Texture);

    glTextureParameteri(Texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(Texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(Texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int Width, Height, NumChannels;
    
    uint8_t* Pixels = stbi_load(Filename.c_str(), &Width, &Height, &NumChannels, 0);
    assert(Pixels != nullptr);

    GLenum InternalFormat, Format;
    switch (NumChannels)
    {
        case 3:
            InternalFormat = GL_RGB8;
            Format = GL_RGB;
            break;

        case 4:
            InternalFormat = GL_RGBA8;
            Format = GL_RGBA;
            break;

        default:
            assert(true);
    }

    glTextureStorage2D(Texture, 1, GL_RGBA8, Width, Height);
    glTextureSubImage2D(Texture, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, Pixels);

    stbi_image_free(Pixels);
}

void Texture_t::Bind() const
{
    glBindTextureUnit(0, Texture);
}