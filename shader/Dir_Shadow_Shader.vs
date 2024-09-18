#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];
uniform bool ifAnimeted;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if(boneIDs[i] == -1)
            continue;
        if(boneIDs[i] >= MAX_BONES || !ifAnimeted)
        {
            totalPosition = vec4(Pos, 1.0f);
            break;
        }
        vec4 localPosition = finalBoneMatrices[boneIDs[i]] * vec4(Pos, 1.0f);
        totalPosition += localPosition * weights[i];
    }
    gl_Position = lightSpaceMatrix * model * totalPosition;
}