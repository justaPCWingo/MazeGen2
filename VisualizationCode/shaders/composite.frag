#version 400

in vec2 texCoords;

uniform sampler2D srcTx;

out vec4 vColor;

void main()
{
    vColor=texture(srcTx,texCoords);
    //vColor.b+=0.2;
    //vColor=vec4(0.,1.,0.,1.);
}

