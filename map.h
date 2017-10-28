#include<glm/glm.hpp>
#include<stdio.h>
#include<vector>
#include<string.h>
#include<string>
#include "buffer.h"
class Map {
	private:
		Buffer vertex,uv,normal;
		int objects=0;
	public:
		void init(const char* name);
		void render();
};