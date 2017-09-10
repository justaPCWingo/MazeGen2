#version 400

layout (location=0) out vec4 vColor;

uniform vec3 pathColor;
void main()
{
    vColor=vec4(pathColor,1.0);
}

