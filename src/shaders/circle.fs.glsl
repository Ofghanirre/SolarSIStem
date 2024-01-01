#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

void main() {
  fFragColor = vec3(1.0, 1.0, 1.0);
};