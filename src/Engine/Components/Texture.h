#pragma once

class Texture
{
public:
    unsigned GetTextureID() const;
    void SetTextureID(unsigned textureID);

    void Activate();

private:
    unsigned _textureID;
};
