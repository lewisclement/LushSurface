#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 focusPosition;

out vec3 fragPosition;
out vec3 Normal;
out float focusDistance;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPosition = vec3(model * vec4(position, 1.0f));
    Normal = normalModel * normal;

    focusDistance = focusPosition.y - position.y;
}
