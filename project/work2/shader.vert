attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 EyePosition;

varying vec4 vv4color;

uniform mat4 MVP;
uniform mat4 ViewTrans;
uniform mat4 ModelTrans;

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

    gl_Position = MVP * Position;

    vec3 model_position_camera = (ViewTrans * ModelTrans * Position).xyz;
    vec3 light_position_camera = (ViewTrans * LightSource[0].position).xyz;

    vec3 N = normalize(transpose(inverse(ModelTrans)) * vec4(Normal, 0)).xyz;
    vec3 L = normalize(light_position_camera);
    vec3 V = normalize(EyePosition - model_position_camera);
    vec3 H = normalize(L + V);
    float attenuation = LightSource[0].constantAttenuation;

    /* Is directional light */
    if (LightSource[0].position.w == 1) {
        L = normalize(light_position_camera - model_position_camera);

        float d = length(L);
        attenuation = min(1, 1 / (
            LightSource[0].constantAttenuation
            + LightSource[0].linearAttenuation * d
            + LightSource[0].quadraticAttenuation * d * d
        ));
    }

    vec4 ambient = Material.ambient * LightSource[0].ambient;
    vec4 diffuse = Material.diffuse * LightSource[0].diffuse * max(dot(L, N), 0);
    vec4 specular = Material.specular * LightSource[0].specular * pow(max(dot(H, N), 0), LightSource[0].spotExponent);

    vv4color = ambient + attenuation * (diffuse + specular);
}
