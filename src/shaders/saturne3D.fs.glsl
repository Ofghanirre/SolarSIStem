#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uSaturneTexture;

void main() {
  fFragColor = texture(uSaturneTexture, vTexCoords).rgb;
};
