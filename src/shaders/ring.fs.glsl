#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uRingTexture;
uniform vec3 uLightSource;

// Paramètres de lumière
const vec3 uLightColor = vec3(1.0, 1.0, 1.0); // Lumière blanche, RGB (1, 1, 1)
const vec3 uAmbientColor = vec3(0.0, 0.0, 0.0); // Pas de composante ambiante

void main() {
  // Normalisation de la normale
  vec3 normal = normalize(vNormalVC);
  vec3 lightDirection = normalize(uLightSource - vPositionVC);


  
  // Calcul de l'intensité diffuse
  float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
  
  vec3 textureColor = texture(uRingTexture, vTexCoords).rgb;

  // Application de l'éclairage diffus
  vec3 finalColor = textureColor * (diffuseIntensity * uLightColor + uAmbientColor);

  // Assignation de la couleur finale
  fFragColor = finalColor;
};
