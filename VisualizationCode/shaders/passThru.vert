#version 400

uniform mat4 projMat;
//uniform mat4 mvMat;

layout (location=0) in vec4 pos;
//layout (location=1) in vec3 norm;


void main()
{
    gl_Position=pos*projMat;
}
