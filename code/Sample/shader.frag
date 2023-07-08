#version 330
struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// 从顶点着色器传来的输入变量（名称相同、类型相同）
struct VS_OUT {
    vec3 FragPos;   // 世界空间的坐标 //
    vec3 Normal;    // 切线空间的面法向 //
    vec2 TexCoords; // 纹理坐标 //
    // 切线空间的光学属性
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
};
in VS_OUT vs_out;


uniform bool blinn;
uniform bool normalMap;

uniform Material material;
uniform Light light;

uniform vec3 objectColor;   // 物体颜色 //

uniform vec3 lightPos;  // 灯光位置 //
uniform vec3 viewPos;   // 相机位置 //

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

out vec4 FragColor;

void main() {
    vec3 normal = vec3(0.0);
    if(normalMap) // 是否使用法线贴图
    {
        normal = texture(texture_normal1, vs_out.TexCoords).rgb;   // 从法线贴图范围[0,1]获取法线
        normal = normalize(normal * 2.0 - 1.0);  // 将法线向量转换为范围[-1,1], this normal is in tangent space
    }
    else
    {
        normal = normalize(vs_out.Normal);
    }

    // get diffuse color
    vec3 color = texture(texture_diffuse1, vs_out.TexCoords).rgb;
    // ambient
    vec3 ambient = light.ambient * color;
    // diffuse
    vec3 lightDir = normalize(vs_out.TangentLightPos - vs_out.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * color;
    // specular
    vec3 viewDir = normalize(vs_out.TangentViewPos - vs_out.TangentFragPos);
    float spec = 0.0;
    if(blinn) // blinn-phong光照模型
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else    // phong光照模型
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    }
    vec3 specular = light.specular * spec * texture(texture_specular1, vs_out.TexCoords).rgb;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
