#include "include/texture.h"
#include "vendor/stb_image/stb_image.h"

uint32_t load_texture(char* path, GLenum type, uint32_t slot) {
    uint32_t texture;
    int32_t width, height, num_channels;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data;
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path, &width, &height, &num_channels, 0);
    if (data) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("failed to load texture!");
    }
    stbi_image_free(data);
    return texture;
}
