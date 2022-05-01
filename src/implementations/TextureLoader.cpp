#include "../headers/includes.h"
#include "../headers/TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned int TextureLoader::LoadTexture(const char *textureFilePath)
{
    stbi_set_flip_vertically_on_load(true);

    int textureWidth = 0;
    int textureHeigth = 0;
    int numberComponents = 0;

    // load texture from disk
    auto textureData = stbi_load(textureFilePath, &textureWidth, &textureHeigth, &numberComponents, 3);

    assert(textureData);

    // STARTING - load to video card
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeigth, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

    // filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);

    // wraping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // create mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    // send texture tu video card
    glBindTexture(GL_TEXTURE_2D, 0);

    // remove image from RAM
    stbi_image_free(textureData);

    return textureId;
}