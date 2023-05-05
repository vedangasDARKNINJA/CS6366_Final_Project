#shader vertex
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;
layout(location=3) in vec3 aTangent;

out V2F
{
    vec3 vWorldPosition;
    vec2 vTexCoords;
    vec3 vTangentLightPos;
    vec3 vTangentCameraPos;
    vec3 vTangentWorldPos;
}v2f;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

/*Ignore*/ uniform vec3 uCameraPosition; 
/*Ignore*/ uniform vec3 uLightPosition;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N)); 

    vec4 worldPos = uModel * vec4(aPos, 1.0);
    gl_Position = uProjection * uView * worldPos;

    v2f.vTexCoords = aTexCoords;
    v2f.vWorldPosition = vec3(worldPos);

    // Tangent Space Calculations
    v2f.vTangentWorldPos = TBN * v2f.vWorldPosition;
    v2f.vTangentCameraPos = TBN * uCameraPosition;
    v2f.vTangentLightPos = TBN * uLightPosition;
}

#shader fragment
#version 330 core
in V2F
{
    vec3 vWorldPosition;
    vec2 vTexCoords;

    vec3 vTangentLightPos;
    vec3 vTangentCameraPos;
    vec3 vTangentWorldPos;
}v2f;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    sampler2D normalMap;
    float shininess;
}; 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material uMaterial;
uniform Light uLight;


out vec4 FragColor;

vec3 calculateAmbient()
{
    return uLight.ambient * texture(uMaterial.diffuse,v2f.vTexCoords).rgb;
}

vec3 calculateDiffuse()
{
    // Sample the normal from the normal map
    vec3 normal = texture(uMaterial.normalMap, v2f.vTexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 lightDir = normalize(v2f.vTangentLightPos - v2f.vTangentWorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    return uLight.diffuse * (diff * texture(uMaterial.diffuse,v2f.vTexCoords).rgb);
}

vec3 calculateSpecular()
{
    // Sample the normal from the normal map
    vec3 normal = texture(uMaterial.normalMap, v2f.vTexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 lightDir = normalize(v2f.vTangentLightPos - v2f.vTangentWorldPos);
    vec3 viewDir = normalize(v2f.vTangentCameraPos - v2f.vTangentWorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.shininess);
    return uLight.specular * (spec * texture(uMaterial.specular, v2f.vTexCoords).rgb);  
}

void main()
{
    vec3 ambient = calculateAmbient();
    vec3 diffuse = calculateDiffuse();
    vec3 specular = calculateSpecular();
    float distance = length(v2f.vTangentLightPos - v2f.vTangentWorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 result = ambient + (diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);

    /*
    vec3 normal = texture(uMaterial.normalMap, v2f.vTexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    //FragColor = vec4(normal,1.0);

    //FragColor = vec4(specular,1.0);
    */
}