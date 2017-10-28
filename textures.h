#ifndef _TEXTURES_H
#define _TEXTURES_H
#include<stdio.h>
#include "lodepng.h"
#include "program.h"
#define GLEW_STATIC
#include<GL/glew.h>
class Texture {
    private:
    GLuint texture;
    public:
    Texture();
    bool loadPNG(const char*);
    void use(Program);
    void destroy();
};
#endif // _TEXTURES_H
