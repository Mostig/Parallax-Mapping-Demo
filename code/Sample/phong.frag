#version 330
out vec4 FragColor;


// 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec3 Normal; // 法向 //
in vec3 FragPos; // 位置 //
in vec2 TexCoords; // 纹理 //

uniform sampler2D diffuseMap;

uniform vec3 lightPos;  // 灯光位置 //
uniform vec3 viewPos;   // 相机位置 //

void main() {
    vec3 normal = normalize(Normal);
    // get diffuse color
    vec3 color = texture(diffuseMap, TexCoords).rgb;

    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}


