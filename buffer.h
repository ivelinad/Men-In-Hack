#ifndef _BUFFER_H
#define _BUFFER_H
#define GLEW_STATIC
#include<GL/glew.h>
class Buffer {
	GLuint bufferID;
	public:
	Buffer();
	~Buffer();
	void writeData(float*,int);
	void use(int,int);
	void free(int);
};
#endif