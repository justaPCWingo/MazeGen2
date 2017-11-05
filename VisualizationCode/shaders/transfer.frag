
#version 400
in vec2 texCoords;

uniform float kernel[41];
uniform int kBound;
uniform vec2 offset;
uniform sampler2D src;

layout(location=0) out vec4 vColor;

void main()
{
    vColor=vec4(0.);
    for(int i=-kBound; i<=kBound; ++i)
    {
        vColor+=kernel[i+kBound]*texture(src,texCoords+(offset*i));
    }
}
