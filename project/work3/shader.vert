attribute vec4 Position;
attribute vec3 Normal;

varying vec4 vv4color;
varying vec4 vv4position;

uniform int  Shading;
uniform vec3 EyePosition;
uniform mat4 MVP;
uniform mat4 ViewTrans;
uniform mat4 ModelTrans;

struct LightSourceParameters {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec4 halfVector;
    vec4 spotDirection;
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
// enum SHADING { GOURAUD, PHONE };

uniform MaterialParameters Material;
uniform LightSourceParameters LightSource[3];


void main() {

    gl_Position = MVP * Position;

    if (Shading == 1) {
        vv4color = vec4(Normal, 0);
        vv4position = Position;
        return;
    }

    vec4 color = vec4(0, 0, 0, 1);

    mat4 ModelViewMatrix = ViewTrans * ModelTrans;
    mat4 NormalMatrix = transpose(inverse(ModelViewMatrix));

    vec3 model_position_camera = (ModelViewMatrix * Position).xyz;

    for (int i = 0; i < 3; ++i) {

        if (LightSource[i].is_on == 0) continue;

        vec4 ambient, diffuse, specular;

        vec3 light_position_camera = (ViewTrans * LightSource[i].position).xyz;

        vec3 N = normalize((NormalMatrix * vec4(Normal, 0)).xyz);
        vec3 L = normalize(light_position_camera);
        vec3 V = normalize(EyePosition - model_position_camera);
        vec3 H = normalize(L + V);
        float attenuation = LightSource[i].constantAttenuation;

        /* For Point Light */
        if (LightSource[i].position.w == 1) {
            L = light_position_camera - model_position_camera;

            float d = length(L);
            attenuation = min(1, 1 / (
                LightSource[i].constantAttenuation
                + LightSource[i].linearAttenuation * d
                + LightSource[i].quadraticAttenuation * d * d
            ));
            L = normalize(L);

            /* For Spotlight */
            if (LightSource[i].spotCutoff <= 90.0) {
                vec3 dir = normalize((transpose(inverse(ViewTrans)) * LightSource[i].spotDirection).xyz);
                float cos_theta = max(0, dot(-L, dir));
                float theta = degrees(acos(cos_theta));
                if (theta > LightSource[i].spotCutoff)
                    attenuation = 0;
                else attenuation *= pow(cos_theta, LightSource[i].spotExponent);
            }
        }

        ambient = Material.ambient * LightSource[i].ambient;
        diffuse = attenuation
          * Material.diffuse
          * LightSource[i].diffuse
          * max(dot(L, N), 0);

        if (dot(L, N) > 0.0) {
            vec3 refVector = reflect(-L, N);
            specular = attenuation
              * Material.specular
              * LightSource[i].specular
              * pow(max(dot(V, refVector), 0), 0.2 * max(Material.shininess, 20));
            // half way vector
            // specular = Material.specular * LightSource[i].specular * pow(max(dot(H, N), 0), 0.2 * max(Material.shininess, 20));
        }

        color += ambient + attenuation * (diffuse + specular);
    }

    vv4color = color;
}
