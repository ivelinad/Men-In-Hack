#ifndef _PROGRAM_H
#define _PROGRAM_H
#include "shader.h"
class Program {
    private:
    GLuint shaders[64];
    int shaderl=0;
    GLuint program;
    public:
    Program();
    bool attachShader(Shader);
    void create();
    void use();
    GLint getUniformLocation(const GLchar*);
    void destroy();
};
#endif