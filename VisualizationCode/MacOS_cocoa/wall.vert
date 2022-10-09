#version 400

//uniform mat4 mvMat;

layout (location=0) in vec4 pos;
layout (location=1) in uint wall;

void main()
{
    vec4 newPos=pos;
    //pack wall data in to w
    newPos.w=0.0f;
    if ((wall & uint(0x1))>0)
        newPos.w+=1.0f;
    if ((wall & uint(0x2))>0)
        newPos.w+=2.0f;
    gl_Position=newPos;
    
}
