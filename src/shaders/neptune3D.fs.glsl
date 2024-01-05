#version 330 core

in vec3 vPositionVC;
in vec3 vNormalVC; 
in vec2 vTexCoords; 

out vec3 fFragColor;

uniform sampler2D uNeptuneTexture;
uniform vec3 uLightSource;

// Paramètres de lumière
const vec3 uLightColor = vec3(1.0, 1.0, 1.0); // Lumière blanche, RGB (1, 1, 1)
const vec3 uAmbientColor = vec3(0.0, 0.0, 0.0); // Pas de composante ambiante
uniform vec3 uViewPosition; // position caméra
uniform float uShininess = 5.0; // facteur de brillance

void main() {
    // Normalisation de la normale
    vec3 normal = normalize(vNormalVC);
    vec3 lightDirection = normalize(uLightSource - vPositionVC);
    
    // Calcul de la direction de la vue
    vec3 viewDirection = normalize(uViewPosition - vPositionVC);

    // Calcul de la moitié de la direction entre la direction de la lumière et la direction de la vue
    vec3 h = normalize(lightDirection + viewDirection);

    // Calcul de l'intensité diffuse
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0) *0.15;

    // Calcul de l'intensité spéculaire (Blinn-Phong)
    float specularIntensity = pow(max(dot(normal, h), 0.0), uShininess);

    // Combinaison de la texture de la terre et de la texture des nuages avec l'éclairage
    vec3 textureColor = texture(uNeptuneTexture, vTexCoords).rgb;

    // Application de l'éclairage diffus
    vec3 finalColor = textureColor * (diffuseIntensity * uLightColor + specularIntensity * uLightColor) + uAmbientColor;

    // Assignation de la couleur finale
    fFragColor = finalColor;
};