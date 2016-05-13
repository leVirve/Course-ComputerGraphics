#include "shaders.h"

#pragma warning (disable: 4996)


void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled)
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, shaderCompiled);
    if (GL_FALSE == (*shaderCompiled))
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character.
        GLchar *errorLog = (GLchar*)malloc(sizeof(GLchar) * maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        fprintf(stderr, "%s", errorLog);

        glDeleteShader(shader);
        free(errorLog);
    }
}

void setShaders()
{
    GLuint v, f, p;
    char *vs = NULL;
    char *fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead("shader.vert");
    fs = textFileRead("shader.frag");

    glShaderSource(v, 1, (const GLchar**)&vs, NULL);
    glShaderSource(f, 1, (const GLchar**)&fs, NULL);

    free(vs);
    free(fs);

    // compile vertex shader
    glCompileShader(v);
    GLint vShaderCompiled;
    showShaderCompileStatus(v, &vShaderCompiled);
    if (!vShaderCompiled) system("pause"), exit(123);

    // compile fragment shader
    glCompileShader(f);
    GLint fShaderCompiled;
    showShaderCompileStatus(f, &fShaderCompiled);
    if (!fShaderCompiled) system("pause"), exit(456);

    p = glCreateProgram();

    // bind shader
    glAttachShader(p, f);
    glAttachShader(p, v);

    // link program
    glLinkProgram(p);

#define GL_BIND(X)  \
    world.R.X = glGetAttribLocation(p, #X)
    GL_BIND(Position);
    GL_BIND(Normal);
#undef GL_BIND

#define GL_BIND(X)  \
    world.R.X = glGetUniformLocation(p, #X)
    GL_BIND(MVP);
    GL_BIND(ModelTrans);
    GL_BIND(ViewTrans);
    GL_BIND(EyePosition);
    GL_BIND(Material.position);
    GL_BIND(Material.ambient);
    GL_BIND(Material.diffuse);
    GL_BIND(Material.specular);
#define GL_LIGHT_BIND(i) \
    GL_BIND(LightSource[i].position);  \
    GL_BIND(LightSource[i].ambient);  \
    GL_BIND(LightSource[i].diffuse);  \
    GL_BIND(LightSource[i].specular);  \
    GL_BIND(LightSource[i].constantAttenuation);  \
    GL_BIND(LightSource[i].linearAttenuation);  \
    GL_BIND(LightSource[i].quadraticAttenuation);  \
    GL_BIND(LightSource[i].spotDirection);  \
    GL_BIND(LightSource[i].spotExponent);  \
    GL_BIND(LightSource[i].spotCutoff);  \
    GL_BIND(LightSource[i].spotCosCutoff);  \
    GL_BIND(LightSource[i].is_on);
    GL_LIGHT_BIND(0);
    GL_LIGHT_BIND(1);
    GL_LIGHT_BIND(2);
#undef GL_LIGHT_BIND
#undef GL_BIND

    glUseProgram(p);

    /* Default light source */
    for (int i = 0; i < 3; ++i) {
        glUniform4fv(world.R.LightSource[i].ambient, 1, world.lights[i].ambient);
        glUniform4fv(world.R.LightSource[i].diffuse, 1, world.lights[i].diffuse);
        glUniform4fv(world.R.LightSource[i].specular, 1, world.lights[i].specular);
        glUniform4fv(world.R.LightSource[i].position, 1, world.lights[i].position);
        glUniform1f(world.R.LightSource[i].spotExponent, world.lights[i].spotExponent);
        glUniform1f(world.R.LightSource[i].constantAttenuation, world.lights[i].constantAttenuation);
        glUniform1i(world.R.LightSource[i].is_on, world.lights[i].is_on);
    }
}

char *textFileRead(char *fn)
{
    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);
            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

int textFileWrite(char *fn, char *s)
{
    FILE *fp;
    int status = 0;
    if (fn != NULL) {
        fp = fopen(fn,"w");
        if (fp != NULL) {
            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s)) {
                status = 1;
            }
            fclose(fp);
        }
    }
    return(status);
}

