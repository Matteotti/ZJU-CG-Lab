#include "Texture.h"

#include <glad/glad.h>

unsigned Texture::GetTextureID() const
{
    return _textureID;
}

void Texture::SetTextureID(unsigned textureID)
{
    _textureID = textureID;
}

void Texture::Activate()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}
