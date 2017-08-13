#version 400

layout(points) in;
layout(max_vertices=24) out;
layout(line_strip) out;

in uint gWall[1];

uniform mat4 projMat;


void BuildCell(vec4 tl)
{
    vec4 v0=vec4(.0f,0.f,0.0f,1.0f);
    vec4 v1=vec4(1.f,0.f,0.0f,1.0f);
    vec4 v3=vec4(1.f,1.f,0.0f,1.0f);
    vec4 v4=vec4(0.f,1.f,0.0f,1.0f);
    
    gl_Position=projMat*(v0+tl);
    EmitVertex();
    gl_Position=projMat*(v1+tl);
    EmitVertex();
    gl_Position=projMat*(v3+tl);
    EmitVertex();
    gl_Position=projMat*(v4+tl);
    EmitVertex();
    gl_Position=projMat*(v0+tl);
    EmitVertex();
    EndPrimitive();
}


void main()
{
    //only draw non-boundary
    if (gWall[0] <4)
    {
        
        BuildCell(vec4(gl_in[0].gl_Position));
        
    }
}
