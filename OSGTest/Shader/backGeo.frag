#version 410 core

in vec2 outTexCor;
out vec4 fragColor;
uniform sampler2D colorTexture;

void main(){
    fragColor=texture(colorTexture,outTexCor);
}