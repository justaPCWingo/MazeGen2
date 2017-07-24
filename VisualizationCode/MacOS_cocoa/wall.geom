#version 400

layout(points) in;
layout(max_vertices=24) out;
layout(triangle_strip) out;

uniform mat4 projMat;


void BuildWall(vec4 start,mat4 mvpMat)
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

    gl_Position=mvpMat*(vec4(.1f,.1f,0.0f,1.0f)+start);
    EmitVertex();
    gl_Position=mvpMat*(vec4(.0f,0.f,0.0f,1.0f)+start);
    EmitVertex();
    gl_Position=mvpMat*(vec4(.9f,.1f,0.0f,1.0f)+start);
    EmitVertex();
    gl_Position=mvpMat*(vec4(1.f,0.f,0.0f,1.0f)+start);
    EmitVertex();
    gl_Position=mvpMat*(vec4(.1f,-.1f,0.0f,1.0f)+start);
    EmitVertex();
    gl_Position=mvpMat*(vec4(.9f,-.1f,0.0f,1.0f)+start);
    EmitVertex();

    
    EndPrimitive();
}

void main()
{
    float wallInfo=gl_in[0].gl_Position.w;
    if (wallInfo>=1.0f)
    {
        BuildWall(vec4(gl_in[0].gl_Position.xyz,1.0f),projMat);
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
        
        BuildWall(vec4(gl_in[0].gl_Position.xyz,1.0f),rotMat*projMat);
    }
    
}
