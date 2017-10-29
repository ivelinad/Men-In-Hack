#include<stdio.h>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform.hpp>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#include<atomic>
#include<thread>
#include "map.h"
#include "program.h"
#include "shader.h"
#include "textures.h"
using namespace glm;
class Engine {
	private:
		GLFWwindow* window=NULL;
		GLuint VertexArrayID;
		std::atomic<float> x{0};
		std::atomic<float> z{0};
		std::atomic<float> angle{0.0f};
		std::atomic<bool> connected{false};
		std::atomic<bool> failure{false};
		std::atomic<bool> running{true};
		std::atomic<float> mouseXDiff{0.0f};
		std::atomic<float> mouseYDiff{0.0f};
		std::atomic<bool> mapChange{false};
		char mapName[32];
	public:
		bool init();
		void server();
		void serverTCP();
		void client(const char* ip);
		void clientTCP(const char* ip);
		void destroy();
		void writeDebug(const char* error);
};