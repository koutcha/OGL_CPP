#version 330 core
out float fragment;
void main(){
	fragment = gl_FragCoord.z;
}