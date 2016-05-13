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
    int is_on;
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

    vec4 r = vec4(0, 0, 0, 1);

    for (int i = 0; i < 3; ++i) {

        if (LightSource[i].is_on == 0) continue;

        vec3 model_position_camera = (ViewTrans * ModelTrans * Position).xyz;
        vec3 light_position_camera = (ViewTrans * LightSource[i].position).xyz;

        vec3 N = normalize(transpose(inverse(ModelTrans)) * vec4(Normal, 0)).xyz;
        vec3 L = normalize(light_position_camera);
        vec3 V = normalize(EyePosition - model_position_camera);
        vec3 H = normalize(L + V);
        float attenuation = LightSource[i].constantAttenuation;

        /* Is directional light */
        if (LightSource[i].position.w == 1) {
            L = normalize(light_position_camera - model_position_camera);

            float d = length(L);
            attenuation = min(1, 1 / (
                LightSource[i].constantAttenuation
                + LightSource[i].linearAttenuation * d
                + LightSource[i].quadraticAttenuation * d * d
            ));
        }

        vec4 ambient = Material.ambient * LightSource[i].ambient;
        vec4 diffuse = Material.diffuse * LightSource[i].diffuse * max(dot(L, N), 0);
        vec4 specular = Material.specular * LightSource[i].specular * pow(max(dot(H, N), 0), LightSource[i].spotExponent);

        r += ambient + attenuation * (diffuse + specular);
    }

    vv4color = r;
}
