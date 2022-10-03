#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Norm;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

out VS_OUT {
    vec3 Normal;
} vs_out;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    vec3 totalTangent = vec3(0.0f);
    vec3 totalBitangent = vec3(0.0f);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(boneIDs[i] == -1)
            continue;
        if(boneIDs[i] >= MAX_BONES)
        {
            totalPosition = vec4(Pos, 1.0f);
            totalNormal = Norm;
            totalTangent = Tangent;
            totalBitangent = Bitangent;
            break;
        }
        vec4 localPosition = finalBoneMatrices[boneIDs[i]] * vec4(Pos, 1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBoneMatrices[boneIDs[i]]) * Norm;
        totalNormal += localNormal * weights[i];
        vec3 localTangent = mat3(finalBoneMatrices[boneIDs[i]]) * Tangent;
        totalTangent += localTangent * weights[i];
        vec3 localBitangent = mat3(finalBoneMatrices[boneIDs[i]]) * Bitangent;
        totalBitangent += localBitangent * weights[i];
    }
    
    vs_out.Normal = mat3(transpose(inverse(model))) * totalNormal;

    gl_Position = projection * view * model * totalPosition;
}