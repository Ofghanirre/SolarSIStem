#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uVenusTexture;

void main() {
  fFragColor = texture(uVenusTexture, vTexCoords).rgb;
};