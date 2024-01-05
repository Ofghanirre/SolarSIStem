#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uSunTexture;
uniform float uHaloIntensity = 0.1; // Intensité du halo
uniform float uHaloFrequency = 5.0; // Fréquence du halo
uniform float uTime; // Temps pour l'oscillation du halo

void main() {
  // Calcul de l'intensité du halo avec une fonction sinus périodique
  float haloIntensity = uHaloIntensity * 0.5 * (1.0 + sin(uHaloFrequency * uTime));

  // Couleur du soleil
  vec3 sunColor = texture(uSunTexture, vTexCoords).rgb;

  // Couleur finale avec le halo
  vec3 finalColor = sunColor + haloIntensity;

  // Assignation de la couleur finale
  fFragColor = finalColor;
}
