#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube uSkyBox;

void main()
{    
    FragColor = texture(uSkyBox, TexCoords);
}
