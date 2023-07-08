#version 330
layout(location = 0) in vec3 aPos; // 位置变量的属性位置值为0
layout(location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// uniform 变量用于存储从 CPU 传递到 GPU 的变量，uniform 变量的值不能从 shader 中修改 //
//三个矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
