#version 330

layout(location = 0) in vec4 position;

out vec3 pass_colour;

void main(void){

	gl_Position = vec4(position.xyz,1.0);
	pass_colour = vec3(1.0);
	
}