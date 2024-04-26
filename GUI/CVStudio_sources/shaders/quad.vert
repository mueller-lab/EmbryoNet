# version 330 core
layout(location = 0) in vec3 v_position; // this is the position of quad vertices
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(v_position, 1.0);
    TexCoord = texCoord;
}