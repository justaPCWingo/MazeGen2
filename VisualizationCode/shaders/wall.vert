#version 400

uniform mat4 projMat;

layout (location=0) in vec3 pos;
layout (location=1) in uint wall;

void main()
{
    vec4 newPos=vec4(pos,1.0);
    //pack wall data in to w
    newPos.w=float(wall);
//    if ((wall & uint(0x1))>0)
//        newPos.w+=1.0f;
//    if ((wall & uint(0x2))>0)
//        newPos.w+=2.0f;
    gl_Position=newPos;
    
}
