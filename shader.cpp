#include "shader.h"
#include<stdio.h>
#include<string.h>
Shader::Shader(const char* filename,GLenum shaderType) {
	FILE* f=fopen(filename,"r");
	char* data=new char[10240];
	int len=fread(data,sizeof(char),10240,f);
	fclose(f);
	data[len]='\0';
    shader=glCreateShader(shaderType);
    glShaderSource(shader,1,(const GLchar**)&data,NULL);
    glCompileShader(shader);
    glGetShaderiv(shader,GL_COMPILE_STATUS,&Result);
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);
    glGetShaderInfoLog(shader,length,NULL,error);
	delete[] data;
}
bool Shader::getResult(char* result,int* length0) {
    if(result!=NULL)
        memcpy(result,error,length);
    if(length0!=NULL)
        memcpy(length0,&length,sizeof(int));
    return Result;
}
GLuint Shader::getShader() {
    return shader;
}
Shader::~Shader() {
	glDeleteShader(shader);
}