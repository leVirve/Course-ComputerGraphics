attribute vec4 av4position;
attribute vec3 av3normal;
attribute vec3 eye_position;

varying vec4 vv4color;

uniform mat4 mvp;
uniform mat4 view_transform;
uniform mat4 model_transform;

struct LightSourceParameters {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec4 halfVector;
    vec3 spotDirection;
    float spotExponent;
    float spotCutoff; // (range: [0.0,90.0], 180.0)
    float spotCosCutoff; // (range: [1.0,0.0],-1.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

struct MaterialParameters {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform MaterialParameters Material;
uniform LightSourceParameters LightSource[3];

void main() {

    vec3 position_world = (model_transform * av4position).xyz;
    vec3 position_camera = (view_transform * model_transform * av4position).xyz;

    vec3 eye_vec = normalize(eye_position - position_camera);
    vec3 light_camera = (view_transform * LightSource[0].position).xyz;
    vec3 light_vec = normalize(light_camera + eye_vec);

    vec3 normal = normalize(transpose(inverse(model_transform)) * vec4(av3normal, 0)).xyz;

    vec4 vv4ambient_D = Material.ambient * LightSource[0].ambient;
    vec4 vv4diffuse_D = Material.diffuse * LightSource[0].diffuse * max(dot(normal, light_vec), 0);
    vec4 vv4specular_D = Material.specular * LightSource[0].specular * max(dot(normalize(light_vec + eye_vec), normal), 0);
    vv4color = vv4ambient_D + vv4diffuse_D + vv4specular_D;

    gl_Position = mvp * av4position;
}
