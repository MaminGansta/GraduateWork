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

#define BUFFER_SIZE 30
layout (std140, binding = 0) uniform ConvolutionKenel
{
    uniform int uWidth;
    uniform int uHeight;
    vec4 uKernel[30];
};

float GetKernelValue(int x, int y)
{
    int pos = y * uWidth + x;
    float val = uKernel[pos / 4][pos % 4];
    return val;
}


void main()
{
    vec2 tex_offset = 1.0f / textureSize(uImage, 0); 
    vec3 result  = vec3(0);

    for (int i = 0; i < uHeight; i++)
    {
        for (int j = 0; j < uWidth; j++)
        {
            int y = i - 1;
            int x = j - 1;
            vec2 offset = vec2(tex_offset.y * y, tex_offset.x * x);
            
            result += texture(uImage, vTexCoords + offset).rgb * GetKernelValue(j, i);
        }
    }

    FragColor = vec4(result, 1.0);
}
