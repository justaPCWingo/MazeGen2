#version 400

layout (location=0) in vec4 pos;

out vec2 texCoords;

void main()
{
    gl_Position=pos;
    
    texCoords=(pos.xy+vec2(1.0))/vec2(2.0);
}
