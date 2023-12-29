#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main() {
  fFragColor = texture(uEarthTexture, vTexCoords).rgb + texture(uCloudTexture, vTexCoords).rgb;
};
