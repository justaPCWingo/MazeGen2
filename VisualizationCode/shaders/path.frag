#version 400

in float threshold;
layout (location=0) out vec4 vColor;

uniform int isBloom;
uniform vec3 pathColor;
uniform float currTime;

void main()
{
    if (currTime<threshold || (isBloom!=0 && currTime>threshold-1.0))
        discard;
    vColor=vec4(pathColor,1.0);

}

