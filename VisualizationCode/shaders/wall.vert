#version 400

uniform mat4 projMat;

layout (location=0) in vec3 pos;
layout (location=1) in uint wall;

out uint gWall;
void main()
{
    vec4 newPos=vec4(pos,1.0);
    
    gWall=wall;
    gl_Position=newPos;
    
}
