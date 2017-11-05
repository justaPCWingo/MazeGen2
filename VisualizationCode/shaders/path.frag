#version 400

in float threshold;
layout (location=0) out vec4 vColor;

uniform int isBloom;
uniform vec3 pathColor;
uniform vec3 decayColor;
uniform float decayDelay;
uniform float currTime;

void main()
{
    if (currTime<threshold || (isBloom!=0 && currTime>threshold+0.25))
        discard;
    vColor=vec4(pathColor,1.0);
    
    float timeDiff=currTime-threshold;
    if (timeDiff>decayDelay)
    {
        vColor=mix(vColor,vec4(decayColor,1.0),clamp(timeDiff-5.0,0.0,1.0));
    }

}

