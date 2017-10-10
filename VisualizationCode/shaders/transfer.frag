
#version 400
in vec2 texCoords;

uniform float kernel[3];
uniform vec2 offset;
uniform sampler2D src;

layout(location=0) out vec4 vColor;

void main()
{
    vColor=vec4(0.,0.,0.,1.);
    for(int i=-1; i<=1; ++i)
    {
        vColor+=kernel[i+1]*texture(src,texCoords+(offset*i));
    }
}
