#version 400

layout(points) in;
layout(max_vertices=24) out;
layout(triangle_strip) out;

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

    vec4 v0=vec4(.1f,.1f,0.0f,1.0f);
    vec4 v1=vec4(.9f,.1f,0.0f,1.0f);
    vec4 v2=vec4(.0f,0.f,0.0f,1.0f);
    vec4 v3=vec4(1.f,0.f,0.0f,1.0f);
    vec4 v4=vec4(.1f,-.1f,0.0f,1.0f);
    vec4 v5=vec4(.9f,-.1f,0.0f,1.0f);
    
    gl_Position=projMat*((rotMat*v0)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v2)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v1)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v3)+start);
    EmitVertex();
    EndPrimitive();
    
    gl_Position=projMat*((rotMat*v2)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v4)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v3)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v5)+start);
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
    float wallInfo=gl_in[0].gl_Position.w;
//    if(wallInfo>0.0)
//        TriTest(vec4(gl_in[0].gl_Position.xyz,1.0));

    if (wallInfo>=1.0f)
    {
        BuildWall(vec4(gl_in[0].gl_Position.xyz,1.0f),mat4(1.0));
        wallInfo-=1.0f;
    }
    if (wallInfo>=2.0f)
    {
        //build rot mat @ 270 degrees.
        //column order
        mat4 rotMat=mat4(vec4(0.0f,-1.f,0.f,0.f),
                         vec4(1.0f,0.f,0.f,0.f),
                         vec4(0.f,0.f,1.f,0.f),
                         vec4(0.f,0.f,0.f,1.f));
        
        BuildWall(vec4(gl_in[0].gl_Position.xyz,1.0f),rotMat);
    }
    
}
