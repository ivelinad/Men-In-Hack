#include<string.h>
#include "engine.h"
int main(int argc, char* argv[]) {
	bool server=true;
	char* clientIP;
	if(argc<2) {
		return 0;
	}
	else if(argc==2) {
		if(strcmp(argv[1],"server")==0) {
			server=true;
		}
		else {
			return 0;
		}
	}
	else if(argc==3) {
		if(strcmp(argv[1],"client")==0) {
			server=false;
			clientIP=argv[2];
		}
	}
	else {
		return 0;
	}
	Engine engine;
	if(!engine.init()) {
		return 0;
	}
	if(server) {
		engine.server();
	}
	else {
		engine.client(clientIP);
	}
	engine.destroy();
	return 0;
}