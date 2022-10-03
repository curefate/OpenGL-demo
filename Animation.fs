#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflection1;
    sampler2D texture_normal1;
    sampler2D texture_height1;//等待实现中
    float shininess;
}; 

// DirLight
struct DirLight {
    vec3 position;
    vec3 target;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D shadowMap;
    mat4 lightSpaceMatrix;
};

// PointLight
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    samplerCube shadowMap;
    float far_plane;
};

// SpotLight
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	mat3 TBN;
} fs_in;

// Material
uniform vec3 viewPos;
uniform Material material;
uniform bool normalMapping;

// Sampler for reflection
uniform samplerCube skybox;

// Funtion to caculate lights
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcReflect(vec3 viewDir, vec3 normal);
float ShadowCalculation_Dir(DirLight light, vec3 norm);
float ShadowCalculation_Point(PointLight light);

// Lights
uniform int DirLight_count;
uniform DirLight dirLight[10];
uniform int PointLight_count;
uniform PointLight pointLight[10];

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    if(normalMapping)
    {
        norm = texture(material.texture_normal1, fs_in.TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = -normalize(fs_in.TBN * norm);
    }
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 result;

    // 定向光
    for(int i = 0; i < DirLight_count; i++)
        result += CalcDirLight(dirLight[i], norm, viewDir);
    // 点光源
    for(int i = 0; i < PointLight_count; i++)
        result += CalcPointLight(pointLight[i], norm, fs_in.FragPos, viewDir);
    // 反射
    result += CalcReflect(viewDir, norm);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 direction = light.target - light.position;
    vec3 lightDir = normalize(-direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色 Blinn-Phone
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // 合并
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoords).rgb;

    return ambient + (diffuse + specular * (1.0 - ShadowCalculation_Dir(light, normal)));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色 Blinn-Phone
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoords).rbg;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + (diffuse + specular * (1.0 - ShadowCalculation_Point(light)));
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色 Blinn-Phone
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 聚焦光强度
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 合并结果
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoords).rbg;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return ambient + diffuse + specular;
}

vec3 CalcReflect(vec3 viewDir, vec3 normal)
{
    vec3 R = reflect(-viewDir, normal);
    vec3 reflectMap = vec3(texture(material.texture_reflection1, fs_in.TexCoords));
    vec3 reflection = vec3(texture(skybox, R).rgb) * reflectMap;

    float diff = max(normalize(dot(normal, viewDir)), 0.0f);
    vec3 diffuse = diff/2 * vec3(texture(material.texture_diffuse1, fs_in.TexCoords));

    vec3 result = reflection + diffuse;
    return result;
}

float ShadowCalculation_Dir(DirLight light, vec3 norm)
{
    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fs_in.FragPos, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(light.shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for(float x = -1; x <= 1; x+=0.5)
    {
        for(float y = -1; y <= 1; y+=0.5)
        {
            float pcfDepth = texture(light.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25.0;
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

float ShadowCalculation_Point(PointLight light)
{
    vec3 fragToLight = fs_in.FragPos - light.position;
    float currentDepth = length(fragToLight);
    vec3 sampleOffsetDirections[20] = vec3[]
    (
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fs_in.FragPos);
    float diskRadius = (1.0 + (viewDistance / light.far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(light.shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= light.far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}