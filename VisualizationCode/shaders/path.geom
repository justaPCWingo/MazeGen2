#version 400

layout(lines_adjacency) in;
layout(max_vertices=24) out;
layout(triangle_strip) out;

in float gThreshold[4];
out float threshold;
uniform mat4 projMat;

const float HWIDTH=0.05;

void BuildSegment(int indL, int indC, int indR,float dir)
{
    vec4 lhs=gl_in[indL].gl_Position;
    vec4 cent=gl_in[indC].gl_Position;
    vec4 rhs=gl_in[indR].gl_Position;
    
    
    vec2 a=normalize(cent.xy-lhs.xy);
    vec2 b=normalize(rhs.xy-cent.xy);
    vec4 lMid=(lhs+cent)*0.5;

    
    float lThresh=(gThreshold[indL]+gThreshold[indC])*0.5;

    
    vec4 sliceStep=vec4(a-b,0.0,0.0)*HWIDTH;
    
    vec4 lStep=normalize(vec4((cent.xy-lhs.xy).yx,0.0,0.0))*HWIDTH*dir;
    
    if (abs(a)==abs(b))
        sliceStep=lStep;
    threshold=lThresh;
    gl_Position=projMat*(lMid+lStep);
    EmitVertex();
    gl_Position=projMat*(lMid-lStep);
    EmitVertex();
    threshold=gThreshold[indC];
    gl_Position=projMat*(cent+lStep);
    EmitVertex();
    gl_Position=projMat*(cent-lStep);
    EmitVertex();
    threshold=gThreshold[indC];
    if (abs(a)!=abs(b))
    {
        threshold=gThreshold[indC]+(gThreshold[indC]-lThresh)*HWIDTH;
        gl_Position=projMat*(cent+sliceStep);
        EmitVertex();
        threshold=gThreshold[indC]-(gThreshold[indC]-lThresh)*HWIDTH;
        gl_Position=projMat*(cent-sliceStep);
        EmitVertex();
    }
    EndPrimitive();
    
}

void main()
{
    BuildSegment(0,1,2,1.0);
    BuildSegment(2,1,0,-1.0);
    BuildSegment(1,2,3,-1.0);
    BuildSegment(3,2,1,1.0);
    //find difference
//    vec2 diff=gl_in[2].gl_Position.xy-gl_in[1].gl_Position.xy;
//    //flip diff to find step direction for verts
//    vec4 step=vec4((diff.yx)*HWIDTH,0.0,0.0);
//
//    //build primitives
//    for (int i=1; i<=2;++i)
//    {
//        threshold=gThreshold[i];
//        gl_Position=projMat*(gl_in[i].gl_Position+step);
//        EmitVertex();
//        threshold=gThreshold[i];
//        gl_Position=projMat*(gl_in[i].gl_Position-step);
//        EmitVertex();
//    }
    
//            threshold=gThreshold[0];
//            gl_Position=projMat*(gl_in[0].gl_Position+step);
//            EmitVertex();
//            threshold=gThreshold[0];
//            gl_Position=projMat*(gl_in[0].gl_Position-step);
//            EmitVertex();
//            threshold=gThreshold[1];
//            gl_Position=projMat*(gl_in[1].gl_Position+step);
//            EmitVertex();
//            threshold=gThreshold[1];
//            gl_Position=projMat*(gl_in[1].gl_Position-step);
//            EmitVertex();


    EndPrimitive();
    
}
