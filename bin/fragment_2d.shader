#version 330 core
/*in vec2 UV;
in vec3 coords;
in vec3 normal;*/
out vec4 color;
//uniform sampler2D textureSampler;

void main() {
	/*vec3 normalVector=normal;
	vec3 lightPos={0.0f,15.0f,0.0f};
	vec3 lightVector=normalize(lightPos-coords);
	float d=distance(coords,lightPos);
	vec4 textureColor=texture(textureSampler,UV).rgba;
	float nDotVP;
	nDotVP=max(0.2f,dot(normalVector,lightVector));
	textureColor=textureColor*nDotVP;
	textureColor.a=1.0f;
	color=textureColor;*/
	//color=vec4(1.0f,0.0f,0.0f,1.0f)*nDotVP;
	color=vec4(0.0f,0.0f,0.0f,1.0f);
}