#shader vertex
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexCoords;

out vec2 vTexCoords;


void main()
{
    gl_Position = vec4(aPos, 1.0);
    vTexCoords = aTexCoords;
}

#shader fragment
#version 330 core
in vec2 vTexCoords;

uniform sampler2D uScreenTexture;

out vec4 FragColor;

void main()
{
    // Gamma Correction
    float gamma = 2.2;
    vec4 result = texture(uScreenTexture,vTexCoords);
    FragColor = vec4(pow(result.rgb,vec3(1.0/gamma)),1.0);
    //FragColor = vec4(result.rgb,1.0);
}