#version 330 core

layout(location=0) in vec3 vertex;
in vec4 osg_Color;


out vec3 color;

void main(){
    color=osg_Color;
    gl_Position=vertex;
}