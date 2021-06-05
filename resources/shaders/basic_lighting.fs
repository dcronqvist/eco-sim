#version 330 core
out vec4 FragColor;

uniform bool u_textures_present;
uniform sampler2D texture_diffuse1;
uniform vec3 u_viewPos;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_material;
uniform Light u_lightDirectional;
uniform vec3 u_slopeColor;
uniform float u_slopeFactor;
uniform float u_sloping;

void main()
{
    // Calculating ambient color
    vec3 ambient = u_material.ambient * u_lightDirectional.ambient;

    // Calculating diffuse color
    vec3 xTangent = dFdx(fragPos);
    vec3 yTangent = dFdy(fragPos);
    vec3 norm = normalize(cross(xTangent, yTangent));

    vec3 up = vec3(0.0, 1.0, 0.0);
    float slope = 1.0 - round(max(dot(norm, up), 0.0) * u_sloping);
    ambient = mix(ambient, u_slopeColor, slope * u_slopeFactor);

    //vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-u_lightDirectional.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * u_material.diffuse) * u_lightDirectional.diffuse;

    // Specular lighting
    vec3 viewDir = normalize(u_viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = (u_material.specular * spec) * u_lightDirectional.specular;

    vec3 result = (ambient + diffuse + specular);
    
    if(u_textures_present) {
        FragColor = vec4(result, 1.0) * texture(texture_diffuse1, texCoords);
    } else {
        FragColor = vec4(result, 1.0);
    }
}