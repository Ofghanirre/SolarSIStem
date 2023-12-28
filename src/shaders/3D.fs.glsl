#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uSunTexture;
uniform sampler2D uMercureTexture;
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;
uniform sampler2D uBackgroudTexture;
uniform samplerCube skybox;

void main() {
  fFragColor = texture(uEarthTexture, vTexCoords).rgb + texture(uCloudTexture, vTexCoords).rgb;
  //fFragColor = vec3(vTexCoords,0.0);
};