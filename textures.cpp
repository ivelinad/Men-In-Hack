#include "textures.h"
Texture::Texture() {
    glGenTextures(1,&texture);
}
bool Texture::loadPNG(const char* filename) {
    std::vector<unsigned char> image;
    unsigned int width,height;
    if(lodepng::decode(image,width,height,filename)!=0)
        return false;
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,&image[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
    return true;
}
void Texture::use(Program t) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    glUniform1i(t.getUniformLocation("textureSampler"),0);
}
void Texture::destroy() {
    glDeleteTextures(1,&texture);
}

