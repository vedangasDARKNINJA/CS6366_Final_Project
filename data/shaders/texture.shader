#shader vertex
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

out V2F
{
    vec3 vWorldPosition;
    vec3 vNormal;
    vec2 vTexCoords;
}v2f;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    v2f.vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    v2f.vTexCoords = aTexCoords;
    v2f.vWorldPosition = vec3(worldPos);
    gl_Position = uProjection * uView * worldPos;
}

#shader fragment
#version 330 core
in V2F
{
    vec3 vWorldPosition;
    vec3 vNormal;
    vec2 vTexCoords;
}v2f;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material uMaterial;
uniform Light uLight;

uniform vec3 uViewPos; 

out vec4 FragColor;

vec3 calculateAmbient()
{
    return uLight.ambient * texture(uMaterial.diffuse,v2f.vTexCoords).rgb;
}

vec3 calculateDiffuse()
{
    vec3 normal = normalize(v2f.vNormal);
    vec3 lightDir = normalize(uLight.position - v2f.vWorldPosition);
    float diff = max(dot(normal, lightDir), 0.0);
    return uLight.diffuse * (diff * texture(uMaterial.diffuse,v2f.vTexCoords).rgb);
}

vec3 calculateSpecular()
{
    // specular
    vec3 normal = normalize(v2f.vNormal);
    vec3 lightDir = normalize(uLight.position - v2f.vWorldPosition);
    vec3 viewDir = normalize(uViewPos - v2f.vWorldPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    return uLight.specular * (spec * texture(uMaterial.specular, v2f.vTexCoords).rgb);  
}

void main()
{
    vec3 ambient = calculateAmbient();
    vec3 diffuse = calculateDiffuse();
    vec3 specular = calculateSpecular();
    float max_distance = 1.5;
    float distance = length(light.position - v2f.vWorldPosition);
    float attenuation = 1.0 / distance * distance;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}