#include "buffer.h"
Buffer::Buffer() {
	glGenBuffers(1,&bufferID);
}
Buffer::~Buffer() {
	glDeleteBuffers(1,&bufferID);
}
void Buffer::writeData(float* data,int size) {
	glBindBuffer(GL_ARRAY_BUFFER,bufferID);
    glBufferData(GL_ARRAY_BUFFER,size*sizeof(float),data,GL_STATIC_DRAW);
}
void Buffer::use(int location,int size) {
	glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER,bufferID);
    glVertexAttribPointer(
        location,
        size,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
}
void Buffer::free(int location) {
	glDisableVertexAttribArray(location);
}