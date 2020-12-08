#shader vertex
#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;

void main()
{
    vTexCoords = aTexCoords;
    gl_Position = vec4(aPosition, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;
  
in vec2 vTexCoords;

uniform sampler2D uImage;

void main()
{
    FragColor = texture(uImage, vTexCoords);
}
