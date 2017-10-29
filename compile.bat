@echo off
SET INCLUDES=-I../Libraries/glfw-3.1.2.bin.WIN32/include -I../Libraries/glew-1.13.0/include -I../Libraries/glm
SET LINKS=-L../Libraries/glfw-3.1.2.bin.WIN32/lib-mingw -L../Libraries/glew-1.13.0/lib
SET LINKING=-static -lglfw3 -lglew32 -lopengl32 -lws2_32 -lwinmm -mwindows
SET PARAMETERS=-std=c++11 -O2 -Wall
g++ -c main.cpp -o obj/main.o %PARAMETERS% %INCLUDES%
g++ -c engine.cpp -o obj/engine.o %PARAMETERS% %INCLUDES%
g++ -c program.cpp -o obj/program.o %PARAMETERS% %INCLUDES%
g++ -c shader.cpp -o obj/shader.o %PARAMETERS% %INCLUDES%
g++ -c map.cpp -o obj/map.o %PARAMETERS% %INCLUDES%
g++ -c buffer.cpp -o obj/buffer.o %PARAMETERS% %INCLUDES%
g++ -c lodepng.cpp -o obj/lodepng.o %PARAMETERS% %INCLUDES%
g++ -c textures.cpp -o obj/textures.o %PARAMETERS% %INCLUDES%
g++ obj/*.o -o bin/Men-In-Hack %PARAMETERS% %LINKS% %LINKING%