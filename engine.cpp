#include "engine.h"

#define M_PI   3.14159265358979323846264338327950288

namespace Sockets {
    bool startup() {
        WSAData wsaData;
        if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
            return false;
        return true;
    }
    void shutdown() {
        WSACleanup();
    }
}
bool startServer(const char* port, SOCKET &ListenSocket) {
    struct addrinfo *result=NULL,hints;
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    hints.ai_flags=AI_PASSIVE;
    if(getaddrinfo(NULL,port,&hints,&result)!=0) {
        return false;
    }
    ListenSocket=INVALID_SOCKET;
    ListenSocket=socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if(ListenSocket==INVALID_SOCKET) {
        freeaddrinfo(result);
        return false;
    }
    if(bind(ListenSocket,result->ai_addr,(int)result->ai_addrlen)==SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        return false;
    }
    freeaddrinfo(result);
    if(listen(ListenSocket,SOMAXCONN)==SOCKET_ERROR) {
        closesocket(ListenSocket);
        return false;
    }
    return true;
}

float distance(float x1, float z1, float x2, float z2) {
	return sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2));
}

bool Engine::init() {
	if(!glfwInit()) {
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES,16);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//window=glfwCreateWindow(1920,1080,"Project Zero",glfwGetPrimaryMonitor(),NULL);
	window=glfwCreateWindow(1920,1080,"Project Zero",NULL,NULL);
	if(!window) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glewExperimental=GL_TRUE;
	if(glewInit()!=GLEW_OK) {
		glfwTerminate();
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1,&VertexArrayID);
	glBindVertexArray(VertexArrayID);
	strcpy(mapName,"default");
	srand(time(0));
	return true;
}

void Engine::server() {
	std::thread thread(&Engine::serverTCP,this);
	while(!connected && !failure) {
		glfwPollEvents();
		if(glfwWindowShouldClose(window)) {
			failure=true;
		}
		Sleep(10);
	}
	if(failure) {
		thread.join();
		return;
	}
	bool quit=false;
	Map map;
	map.init(mapName);
	Map tray;
	tray.init("");
	Program program;
	program.attachShader(Shader("vertex.shader",GL_VERTEX_SHADER));
	program.attachShader(Shader("fragment.shader",GL_FRAGMENT_SHADER));
	program.create();
	Program program2D;
	program2D.attachShader(Shader("vertex_2d.shader",GL_VERTEX_SHADER));
	program2D.attachShader(Shader("fragment_2d.shader",GL_FRAGMENT_SHADER));
	program2D.create();
	Program program2;
	program2.attachShader(Shader("vertex.shader",GL_VERTEX_SHADER));
	program2.attachShader(Shader("fragment.shader",GL_FRAGMENT_SHADER));
	program2.create();
	mat4 Projection=perspective(45.0f,1920.0f/1080.0f,0.1f,1000.0f);
    mat4 Model=mat4(1.0f);
    mat4 View;
    mat4 MVP;
	mat4 TrayProjection=perspective(45.0f,1920.0f/1080.0f,0.1f,1000.0f);
	mat4 TrayModel=mat4(1.0f);
	mat4 TrayView;
	mat4 TrayMVP;
	double mousex,mousey;
	Buffer vertex;
	double begin,end;
	glfwSetCursorPos(window,960,540);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
	while(!quit) {
		float temp_x=x,temp_z=z;
		begin = glfwGetTime();
		if(glfwWindowShouldClose(window)) {
			quit=true;
			break;
		}
		if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS) {
			quit=true;
			break;
		}
		if(!connected) {
			quit=true;
			break;
		}
		if(mapChange) {
			map.init(mapName);
			if(strcmp(mapName,"constructionsite")==0)
				tray.init("bricks");
			else if(strcmp(mapName,"default")==0)
				tray.init("");
			else
				tray.init("tray");
			x=0.0f;
			z=0.0f;
			if(strcmp(mapName,"room")==0) {
				x=-2.0f;
				z=0.0f;
			}
			angle=0.0f;
			mouseXDiff=0.0f;
			mouseYDiff=0.0f;
			mapChange=false;
		}
		if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS) {
			x=x+0.25f*cos(angle);
			z=z+0.25f*sin(angle);
		}
		if(glfwGetKey(window,GLFW_KEY_U)==GLFW_PRESS) {
			strcpy(mapName,"default");
			mapChange=true;
		}
		if(glfwGetKey(window,GLFW_KEY_I)==GLFW_PRESS) {
			strcpy(mapName,"constructionsite");
			mapChange=true;
		}
		if(glfwGetKey(window,GLFW_KEY_O)==GLFW_PRESS) {
			strcpy(mapName,"room");
			mapChange=true;
		}
		if(glfwGetKey(window,GLFW_KEY_G)==GLFW_PRESS) {
			FILE* f=fopen("debug2.txt","a");
			fprintf(f,"%f %f\n",temp_x,temp_z);
			fclose(f);
		}
		if(strcmp(mapName,"default")==0 && distance(temp_x,temp_z,-15.0f,7.0f)<4.0f) {
			strcpy(mapName,"constructionsite");
			mapChange=true;
		}
		else if(strcmp(mapName,"default")==0 && distance(temp_x,temp_z,23.0f,18.0f)<4.0f) {
			strcpy(mapName,"room");
			mapChange=true;
		}
		else if(strcmp(mapName,"constructionsite")==0 && distance(temp_x,temp_z,20.0f,20.0f)<4.0f) {
			strcpy(mapName,"default");
			mapChange=true;
		}
		else if(strcmp(mapName,"default") && (mouseXDiff<-300 || mouseXDiff>300 || mouseYDiff<-300 || mouseYDiff>300)) {
			strcpy(mapName,"default");
			mapChange=true;
		}
		float reach_x = ((mouseXDiff)*2.0f/1920.0f-1.0f);
		float reach_y = ((1080-mouseYDiff)*2.0f/1080.0f-1.0f);
		float point_x = ((1920.0f/2.0f+reach_x*64.0f)/1920.0f*2.0f)-1.0f;
		float point_y = ((1080.0f/2.0f+reach_y*36.0f)/1080.0f*2.0f)-1.0f;
		GLfloat vertexData[18]={point_x-0.05f,point_y-0.05f,0.0f,
								point_x-0.05f,point_y+0.05f,0.0f,
								point_x+0.05f,point_y+0.05f,0.0f,
								
								point_x-0.05f,point_y-0.05f,0.0f,
								point_x+0.05f,point_y+0.05f,0.0f,
								point_x+0.05f,point_y-0.05f,0.0f};
		vertex.writeData(vertexData,18);
		glfwGetCursorPos(window,&mousex,&mousey);
		glfwSetCursorPos(window,960,540);
		angle=angle+((mousex-960)/1920.0f)*2*M_PI;
		glClearColor(0.52f,0.80f,0.97f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		program.use();
		View=lookAt(vec3(x,2.0f,z),vec3(x+cos(angle)*2.0f,0.0f,z+sin(angle)*2.0f),vec3(0,1,0));
        MVP=Projection*View*Model;
		glUniformMatrix4fv(program.getUniformLocation("MVP"),1,GL_FALSE,&MVP[0][0]);
		map.render(program);
		program2.use();
		TrayView=lookAt(vec3(1.5f,1.5f,0.0f),vec3(0.0f,0.0f,0.0f),vec3(0,1,0));
		TrayModel=rotate((reach_x+1.0f)*30.0f*2.0f*(float)(M_PI)/180.0f,vec3(1,0,0))*rotate((reach_y-1.0f)*30.0f*2.0f*(float)(M_PI)/180.0f,vec3(0,0,1));
		TrayMVP=TrayProjection*TrayView*TrayModel;
		glUniformMatrix4fv(program2.getUniformLocation("MVP"),1,GL_FALSE,&TrayMVP[0][0]);
		tray.render(program2);
		program2D.use();
		vertex.use(0,3);
		glDrawArrays(GL_TRIANGLES,0,6);
		vertex.free(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		end = glfwGetTime();
		if(end-begin<0.016f) {
			int time = (0.016f - end + begin) * 1000;
			Sleep(time);
		}
	}
	program.destroy();
	program2D.destroy();
	running=false;
	thread.join();
}

void Engine::serverTCP() {
	SOCKET ListenSocket = INVALID_SOCKET;
	if(!Sockets::startup()) {
		failure=true;
		return;
	}
	if(!startServer("4500",ListenSocket)) {
		Sockets::shutdown();
		failure=true;
		return;
	}
    struct sockaddr_in client_info={0};
    int addrsize=sizeof(client_info);
	SOCKET ClientSocket=INVALID_SOCKET;
	ClientSocket=accept(ListenSocket,(struct sockaddr*)&client_info,&addrsize);
	if(ClientSocket==INVALID_SOCKET) {
		closesocket(ListenSocket);
		failure=true;
		return;
	}
	int t;
	connected=true;
	char buffer[1024];
	while(running) {
		int size=sizeof(float);
		float temp_x=x;
		float temp_z=z;
		float temp_angle=angle;
		memcpy(buffer,&temp_x,size);
		memcpy(buffer+size,&temp_z,size);
		memcpy(buffer+2*size,&temp_angle,size);
		memcpy(buffer+3*size,mapName,32);
        t=send(ClientSocket,buffer,size*3+32,0);
		if(t==SOCKET_ERROR) {
			failure=true;
			break;
		}
		t=recv(ClientSocket,buffer,size*2,0);
		if(t!=size*2) {
			failure=true;
			break;
		}
		float diffX,diffY;
		memcpy(&diffX,buffer,size);
		memcpy(&diffY,buffer+size,size);
		mouseXDiff=mouseXDiff+diffX;
		if(mouseXDiff<-960) mouseXDiff=-960;
		if(mouseXDiff>960) mouseXDiff=960;
		mouseYDiff=mouseYDiff+diffY;
		if(mouseYDiff<-540) mouseYDiff=-540;
		if(mouseYDiff>540) mouseYDiff=540;
	}
	closesocket(ClientSocket);
    closesocket(ListenSocket);
	connected=false;
	Sockets::shutdown();
}

void Engine::client(const char* ip) {
	std::thread thread(&Engine::clientTCP,this,ip);
	while(!connected && !failure) {
		glfwPollEvents();
		if(glfwWindowShouldClose(window)) {
			failure=true;
		}
		Sleep(10);
	}
	if(failure) {
		thread.join();
		return;
	}
	bool quit=false;
	Map map;
	map.init(mapName);
	Program program;
	program.attachShader(Shader("vertex.shader",GL_VERTEX_SHADER));
	program.attachShader(Shader("fragment.shader",GL_FRAGMENT_SHADER));
	program.create();
	mat4 Projection=perspective(45.0f,1920.0f/1080.0f,0.1f,1000.0f);
    mat4 Model=mat4(1.0f);
    mat4 View;
    mat4 MVP;
	glfwSetCursorPos(window,960,540);
	double begin,end;
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
	while(!quit) {
		begin = glfwGetTime();
		if(glfwWindowShouldClose(window)) {
			quit=true;
			break;
		}
		if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS) {
			quit=true;
			break;
		}
		
		if(!connected) {
			quit=true;
			break;
		}
		if(mapChange) {
			map.init(mapName);
			mouseXDiff=0;
			mouseYDiff=0;
			mapChange=false;
		}
		double temp_x,temp_y;
		glfwGetCursorPos(window,&temp_x,&temp_y);
		glfwSetCursorPos(window,960,540);
		mouseXDiff=mouseXDiff+(960-temp_x);
		mouseYDiff=mouseYDiff+(temp_y-540);
		int disturbance_x = rand()%32;
		float fdist_x = disturbance_x/1.0f-16.0f;
		int disturbance_y = rand()%32;
		float fdist_y = disturbance_y/1.0f-16.0f;
		mouseXDiff=mouseXDiff+fdist_x;
		if(mouseXDiff<-960) mouseXDiff=-960;
		if(mouseXDiff>960) mouseXDiff=960;
		if(mouseYDiff<-540) mouseYDiff=-540;
		if(mouseYDiff>540) mouseYDiff=540;
		mouseYDiff=mouseYDiff+fdist_y;
		glClearColor(0.52f,0.80f,0.97f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		View=lookAt(vec3(x,3.5f,z),vec3(x+cos(angle)*32.0f,6.0f,z+sin(angle)*32.0f),vec3(0,1,0));
        MVP=Projection*View*Model;
		program.use();
        glUniformMatrix4fv(program.getUniformLocation("MVP"),1,GL_FALSE,&MVP[0][0]);
		map.render(program);
		glfwSwapBuffers(window);
		glfwPollEvents();
		end = glfwGetTime();
		if(end-begin<0.016f) {
			int time = (0.016f - end + begin) * 1000;
			Sleep(time);
		}
	}
	program.destroy();
	running=false;
	thread.join();
}

void Engine::clientTCP(const char* ip) {
	if(!Sockets::startup()) {
		failure=true;
		return;
	}
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result=NULL,*ptr=NULL,hints;
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    if(getaddrinfo(ip,"4500",&hints,&result)!=0) {
        failure=true;
        return;
    }
    ptr=result;
    ConnectSocket=socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
    if(ConnectSocket==INVALID_SOCKET) {
        failure=true;
        return;
    }
    if(connect(ConnectSocket,ptr->ai_addr,(int)(ptr->ai_addrlen))==SOCKET_ERROR) {
        failure=true;
        return;
    }
    freeaddrinfo(result);
    if(ConnectSocket==INVALID_SOCKET) {
        failure=true;
        return;
    }
	connected=true;
	char buffer[1024];
	int t;
	while(running) {
		int size = sizeof(float);
		t=recv(ConnectSocket,buffer,size*3+32,0);
		if(t!=size*3+32) {
			failure=true;
			break;
		}
		float temp_x,temp_z,temp_angle;
		char tempName[32];
		memcpy(&temp_x,buffer,size);
		memcpy(&temp_z,buffer+size,size);
		memcpy(&temp_angle,buffer+2*size,size);
		memcpy(tempName,buffer+3*size,32);
		x=temp_x;
		z=temp_z;
		angle=temp_angle;
		if(strcmp(tempName,mapName)) {
			strcpy(mapName,tempName);
			mapChange=true;
		}
		float temp_xdiff=mouseXDiff,temp_ydiff=mouseYDiff;
		mouseXDiff=0;
		mouseYDiff=0;
		memcpy(buffer,&temp_xdiff,size);
		memcpy(buffer+size,&temp_ydiff,size);
		t=send(ConnectSocket,buffer,size*2,0);
		if(t==SOCKET_ERROR) {
			failure=true;
			break;
		}
	}
	closesocket(ConnectSocket);
	connected=false;
	Sockets::shutdown();
}

void Engine::destroy() {
	glfwDestroyWindow(window);
	glDeleteVertexArrays(1,&VertexArrayID);
	glfwTerminate();
}

void Engine::writeDebug(const char* error) {
	FILE* f=fopen("debug.txt","w");
	fprintf(f,"%s\n",error);
	fclose(f);
}