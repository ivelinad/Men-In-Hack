#ifndef _SHADER_H
#define _SHADER_H
#define GLEW_STATIC
#include<GL/glew.h>
class Shader {
    public:
    GLuint shader;
    GLint Result=GL_FALSE;
    char error[1024];
    int length=0;
    public:
    Shader(const char*,GLenum);
    bool getResult(char*,int*);
    GLuint getShader();
	~Shader();
};
#endif