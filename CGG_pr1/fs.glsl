#version 400
out vec4 frag_color;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    frag_color = texture(texture_diffuse1, TexCoords);
}