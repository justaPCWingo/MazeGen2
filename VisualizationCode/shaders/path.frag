#version 400

in float threshold;
layout (location=0) out vec4 vColor;

uniform vec3 pathColor;
uniform float currTime;

void main()
{
    if (currTime<threshold)
        discard;
    vColor=vec4(pathColor,1.0);
}

