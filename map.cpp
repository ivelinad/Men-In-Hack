#include "map.h"

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	//printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}
	fclose(file);
	return true;
}

void Map::init(const char* name) {
	char* filename = new char[strlen(name)+5];
	strcpy(filename,name);
	strcat(filename,".obj");
	std::vector<glm::vec3> v_vertex;
	std::vector<glm::vec2> v_uv;
	std::vector<glm::vec3> v_normal;
	loadOBJ(filename,v_vertex,v_uv,v_normal);
	GLfloat* vertexData = new GLfloat[v_vertex.size()*3];
	for(unsigned int i=0;i<v_vertex.size();i++) {
		vertexData[i*3]=v_vertex[i].x;
		vertexData[i*3+1]=v_vertex[i].y;
		vertexData[i*3+2]=v_vertex[i].z;
	}
	vertex.writeData(vertexData,v_vertex.size()*3);
	delete[] vertexData;
	GLfloat* uvData = new GLfloat[v_uv.size()*2];
	for(unsigned int i=0;i<v_uv.size();i++) {
		uvData[i*2]=v_uv[i].x;
		uvData[i*2+1]=v_uv[i].y;
	}
	uv.writeData(uvData,v_uv.size()*2);
	delete[] uvData;
	GLfloat* normalData = new GLfloat[v_normal.size()*3];
	for(unsigned int i=0;i<v_normal.size();i++) {
		normalData[i*3]=v_normal[i].x;
		normalData[i*3+1]=v_normal[i].y;
		normalData[i*3+2]=v_normal[i].z;
	}
	normal.writeData(normalData,v_normal.size()*3);
	delete[] normalData;
	objects = v_vertex.size();
}

void Map::render() {
	vertex.use(0,3);
	uv.use(1,2);
	normal.use(2,3);
	glDrawArrays(GL_TRIANGLES,0,objects);
	vertex.free(0);
	uv.free(1);
	normal.free(2);
}