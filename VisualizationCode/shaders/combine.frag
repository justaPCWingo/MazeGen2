#version 400
in vec2 texCoords;

//uniform sampler2D srcBase;
uniform sampler2D src16;
uniform sampler2D src4;
uniform sampler2D src2;
uniform sampler2D src1;
uniform float intense;

out vec4 bloom; //vColor;

void main()
{
    
    bloom=texture(src16,texCoords)*intense;
    bloom+=texture(src4,texCoords)*intense;
    bloom+=texture(src2,texCoords)*intense;
    bloom+=texture(src1,texCoords)*intense;
    
    //bloom=vec4(1.0,.10,0.0,1.0);
    //vColor=vec4(smoothstep(texture2D(srcBase,texCoords).xyz,bloom.xyz,1.0-bloom.a),1.0);
}

