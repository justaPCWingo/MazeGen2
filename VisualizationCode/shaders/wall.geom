#version 400

layout(points) in;
layout(max_vertices=24) out;
layout(triangle_strip) out;

in uint gWall[1];

uniform mat4 projMat;


void BuildWall(vec4 start,mat4 rotMat)
{
    //   0          1
    //   *__________*
    //  /            \
    // * 2            * 3
    //  \ __________ /
    //   *          *
    //   4          5
    
//    vec4 verts[6]={vec4(.1f,.1f,0.0f,1.0f),
//                   vec4(.9f,.1f,0.0f,1.0f),
//                   vec4(.0f,0.f,0.0f,1.0f),
//                   vec4(1.f,0.f,0.0f,1.0f),
//                   vec4(.1f,-.1f,0.0f,1.0f),
//                   vec4(.9f,-.1f,0.0f,1.0f)};

    mat4 scaleMat=mat4(vec4(2.f,0.f,0.f,0.f),
                       vec4(0.0f,2.f,0.f,0.f),
                       vec4(0.f,0.f,1.f,0.f),
                       vec4(0.f,0.f,0.f,1.f));
    mat4 mvpMat=projMat*scaleMat;

    vec4 v0=vec4(.1f,.1f,0.0f,1.0f);
    vec4 v1=vec4(.9f,.1f,0.0f,1.0f);
    vec4 v2=vec4(.0f,0.f,0.0f,1.0f);
    vec4 v3=vec4(1.f,0.f,0.0f,1.0f);
    vec4 v4=vec4(.1f,-.1f,0.0f,1.0f);
    vec4 v5=vec4(.9f,-.1f,0.0f,1.0f);
    
    gl_Position=mvpMat*((rotMat*v0)+start);
    EmitVertex();
    gl_Position=mvpMat*((rotMat*v2)+start);
    EmitVertex();
    gl_Position=mvpMat*((rotMat*v1)+start);
    EmitVertex();
    gl_Position=mvpMat*((rotMat*v3)+start);
    EmitVertex();
    EndPrimitive();
    
    gl_Position=mvpMat*((rotMat*v2)+start);
    EmitVertex();
    gl_Position=mvpMat*((rotMat*v4)+start);
    EmitVertex();
    gl_Position=mvpMat*((rotMat*v3)+start);
    EmitVertex();
    gl_Position=mvpMat*((rotMat*v5)+start);
    EmitVertex();
    EndPrimitive();
}

void TriTest(vec4 start)
{
    gl_Position=projMat*start;
    EmitVertex();
    vec4 curr=start;
    curr.y+=-1.0;
    gl_Position=projMat*curr;
    EmitVertex();
    curr=start;
    curr.x+=1.0;
    gl_Position=projMat*curr;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    
    if ((gWall[0] & uint(2) )>0)
    {
        
        //build rot mat @ 90 degrees.
        //column order
        mat4 rotMat=mat4(vec4(0.0f,1.f,0.f,0.f),
                         vec4(-1.0f,0.f,0.f,0.f),
                         vec4(0.f,0.f,1.f,0.f),
                         vec4(0.f,0.f,0.f,1.f));
        
        BuildWall(gl_in[0].gl_Position,rotMat);
        
    }
    
    if ((gWall[0] & uint(1) )>0)
        BuildWall(gl_in[0].gl_Position,mat4(1.0));
    
    //BuildWall(vec4(float(gWall[0])*0.001,gl_in[0].gl_Position.yz,1.0f),mat4(1.0));
}
