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

    if (Shading == 0) {
        gl_FragColor = vv4color;
        return;
    }

    vec4 color = vec4(0, 0, 0, 1);

    for (int i = 0; i < 3; ++i) {

        if (LightSource[i].is_on == 0) continue;

        vec4 ambient, diffuse, specular;

        vec3 model_position_camera = (ViewTrans * ModelTrans * vv4position).xyz;
        vec3 light_position_camera = (ViewTrans * LightSource[i].position).xyz;

        vec3 N = normalize((transpose(inverse(ModelTrans)) * vv4color).xyz);
        vec3 L = normalize(light_position_camera);
        vec3 V = normalize(EyePosition - model_position_camera);
        vec3 H = normalize(L + V);
        float attenuation = LightSource[i].constantAttenuation;

        /* For Point Light */
        if (LightSource[i].position.w == 1) {
            L = normalize(light_position_camera - model_position_camera);

            float d = length(L);
            attenuation = min(1, 1 / (
                LightSource[i].constantAttenuation
                + LightSource[i].linearAttenuation * d
                + LightSource[i].quadraticAttenuation * d * d
            ));

            /* For Spotlight */
            if (LightSource[i].spotCutoff != 0) {
                vec3 v = -L;
                vec3 dir = normalize((transpose(inverse(ViewTrans)) * LightSource[i].spotDirection).xyz);
                float theta = degrees(acos(dot(v, dir)));
                if (theta < LightSource[i].spotCutoff) {
                    attenuation *= pow(max(dot(v, dir), 0), LightSource[i].spotExponent);
                } else attenuation = 0;
            }
        }

        ambient = Material.ambient * LightSource[i].ambient;
        diffuse = Material.diffuse * LightSource[i].diffuse * max(dot(L, N), 0);
        if (dot(L, N) != 0.0) {
            vec3 refVector = reflect(-L, N);
            specular = Material.specular * LightSource[i].specular * pow(max(dot(V, refVector), 0), 0.2 * max(Material.shininess, 20));
            // half way vector
            // specular = Material.specular * LightSource[i].specular * pow(max(dot(H, N), 0), 10);
        }

        color += ambient + attenuation * (diffuse + specular);
    }

    gl_FragColor = color;
}
