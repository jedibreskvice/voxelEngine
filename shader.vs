#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;
layout(location = 1) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform the normal vector to account for model transformations
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass the texture coordinates
    TexCoord = aTexCoord;
    
    // Calculate the final position of the vertex
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


