#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in float texID;

uniform sampler2D textures[2];

void main()
{
    int index = int(texID);
    FragColor = texture(textures[index], texCoord);
    //FragColor = vec4(texID, texID, texID, 1.0);
} 