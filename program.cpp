#include "program.h"
Program::Program() {
    program=glCreateProgram();
}
bool Program::attachShader(Shader s) {
    char error[1024];
    int length=0;
    if(s.getResult(error,&length)==false) {
        glDeleteShader(s.getShader());
        return false;
    }
    glAttachShader(program,s.getShader());
    shaders[shaderl++]=s.getShader();
    return true;
}
void Program::create() {
    glLinkProgram(program);
    for(int i=0;i<shaderl;i++)
        glDeleteShader(shaders[i]);
}
void Program::use() {
    glUseProgram(program);
}
GLint Program::getUniformLocation(const GLchar* name) {
    return glGetUniformLocation(program,name);
}
void Program::destroy() {
    glDeleteProgram(program);
}