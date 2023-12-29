#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uSunTexture;

void main() {
  fFragColor = texture(uSunTexture, vTexCoords).rgb;
};