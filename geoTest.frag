#version 330

in vec3 finalColour;

out vec4 fragment;

void main(void){

	fragment = vec4(finalColour, 1.0);

}