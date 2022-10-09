#version 400

layout(points) in;
layout(max_vertices=24) out;
layout(line_strip) out;

in uint gWall[1];

uniform mat4 projMat;


void BuildCell(vec4 tl)
{
    mat4 scaleMat=mat4(vec4(2.f,0.f,0.f,0.f),
                       vec4(0.0f,2.f,0.f,0.f),
                       vec4(0.f,0.f,1.f,0.f),
                       vec4(0.f,0.f,0.f,1.f));
    mat4 mvpMat=projMat*scaleMat;
    
    vec4 v0=vec4(.0f,0.f,0.0f,1.0f);
    vec4 v1=vec4(1.f,0.f,0.0f,1.0f);
    vec4 v3=vec4(1.f,1.f,0.0f,1.0f);
    vec4 v4=vec4(0.f,1.f,0.0f,1.0f);
    
    gl_Position=mvpMat*(v0+tl);
    EmitVertex();
    gl_Position=mvpMat*(v1+tl);
    EmitVertex();
    gl_Position=mvpMat*(v3+tl);
    EmitVertex();
    gl_Position=mvpMat*(v4+tl);
    EmitVertex();
    gl_Position=mvpMat*(v0+tl);
    EmitVertex();
    EndPrimitive();
}


void main()
{
    //only draw non-boundary
    if (gWall[0] <4)
    {
        
        BuildCell(gl_in[0].gl_Position);
        
    }
}
