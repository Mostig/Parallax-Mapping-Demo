#version 330
layout(location = 0) in vec3 aPos; // 位置变量的属性位置值为0
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;   // 材质坐标变量的属性位置值为2
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

struct VS_OUT {
    vec3 FragPos;   // 世界空间的坐标 //
    vec3 Normal;    // 切线空间的面法向 //
    vec2 TexCoords; // 纹理坐标 //
    // 切线空间的光学属性
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
};
out VS_OUT vs_out;

uniform bool normal;
// uniform 变量用于存储从 CPU 传递到 GPU 的变量，uniform 变量的值不能从 shader 中修改 //
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    // 最好预先计算出法向矩阵 //
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N); //对TBN向量进行重正交化(格拉姆-施密特正交化)
    vec3 B = cross(N, T);

    vs_out.Normal = N; //切线空间中的三角面法线

    //创建TBN矩阵:把切线坐标空间的向量转换到世界坐标空间
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
