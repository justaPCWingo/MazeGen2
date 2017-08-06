#version 400

layout (location=0) out vec4 vColor;

uniform vec3 wallColor;
void main()
{
    vColor=vec4(wallColor,1.0);
}

