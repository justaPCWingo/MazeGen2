#version 400
in vec2 texCoords;

//uniform sampler2D srcBase;
uniform sampler2D src1024;
uniform sampler2D src512;
uniform sampler2D src256;
uniform sampler2D src128;

out vec4 bloom; //vColor;

void main()
{
    vec4 bloom=texture(src1024,texCoords)*0.25;
    bloom+=texture(src512,texCoords)*0.25;
    bloom+=texture(src256,texCoords)*0.25;
    bloom+=texture(src128,texCoords)*0.25;
    
    
    //vColor=vec4(smoothstep(texture2D(srcBase,texCoords).xyz,bloom.xyz,1.0-bloom.a),1.0);
}

