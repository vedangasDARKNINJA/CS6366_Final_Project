#shader vertex
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;
layout(location=3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    gl_Position = uProjection * uView * worldPos;
    TexCoords = aTexCoords;
    WorldPos = vec3(worldPos);
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    Normal = normalMatrix * aNormal;
    vec3 normal = normalize(Normal);
    vec3 tangent = normalize(normalMatrix * aTangent);
    vec3 bitangent = normalize(cross(normal,tangent));
    TBN = mat3(tangent, bitangent, normal);
}

#shader fragment
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
in mat3 TBN;

/*Ignore*/uniform vec3 uCameraPosition;

// material parameters
uniform sampler2D uAlbedoMap;
uniform sampler2D uNormalMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uRoughnessMap;
uniform sampler2D uAOMap;

/*Color*/ uniform vec3 uAlbedo;
/*Ignore*/ uniform float uMetallic;
/*Ignore*/ uniform float uRoughness;
uniform float uAO;

uniform bool uUseTextures;
uniform bool uUseToneMapping;

// lights
uniform vec3 uLightPositions[4];
uniform vec3 uLightColors[4];

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(uNormalMap, TexCoords).xyz * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{		
    vec3 albedo     = uUseTextures ? texture(uAlbedoMap, TexCoords).rgb : uAlbedo;
    float metallic  = uUseTextures ? texture(uMetallicMap, TexCoords).r :  uMetallic;
    float roughness = uUseTextures ? texture(uRoughnessMap, TexCoords).r :  uRoughness;
    float ao        = uUseTextures ? texture(uAOMap, TexCoords).r : uAO;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(uCameraPosition - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(uLightPositions[i]- WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(uLightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = uLightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;

        // Satisfy the energy conservation principle
        vec3 kD = vec3(1.0) - kS;

        // Metallic surfaces don't refract light and thus have no diffuse reflections
        // We compensate for this property as follows.
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }   
    
    // This will get a sampled value from environment map when IBL is implemented
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;
	
    // HDR tonemapping
    if(uUseToneMapping)
    {
        color = color / (color + vec3(1.0));
        // Gamma correction in framebuffer 
        //color = pow(color, vec3(1.0/2.2));
    }
    FragColor = vec4(color, 1.0);
}