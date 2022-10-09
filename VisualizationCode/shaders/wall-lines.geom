#version 400

layout(points) in;
layout(max_vertices=24) out;
layout(line_strip) out;

in uint gWall[1];

uniform mat4 projMat;


void BuildWall(vec4 start,mat4 rotMat)
{
    vec4 v0=vec4(.0f,0.f,0.0f,1.0f);
    vec4 v1=vec4(1.f,0.f,0.0f,1.0f);
    
    gl_Position=projMat*((rotMat*v0)+start);
    EmitVertex();
    gl_Position=projMat*((rotMat*v1)+start);
    EmitVertex();
    EndPrimitive();
}


void main()
{
    
    if ((gWall[0] & 2 )>0)
    {
        
        //build rot mat @ 90 degrees.
        //column order
        mat4 rotMat=mat4(vec4(0.0f,1.f,0.f,0.f),
                         vec4(-1.0f,0.f,0.f,0.f),
                         vec4(0.f,0.f,1.f,0.f),
                         vec4(0.f,0.f,0.f,1.f));
        
        BuildWall(vec4(gl_in[0].gl_Position.xyz,1.0f),rotMat);
        
    }
    
    if ((gWall[0] & 1 )>0)
        BuildWall(vec4(gl_in[0].gl_Position.xyz,1.0f),mat4(1.0));
    
    //BuildWall(vec4(float(gWall[0])*0.001,gl_in[0].gl_Position.yz,1.0f),mat4(1.0));
}
