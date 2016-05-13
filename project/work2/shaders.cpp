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
    GL_BIND(LightSource[0].position);
    GL_BIND(LightSource[0].ambient);
    GL_BIND(LightSource[0].diffuse);
    GL_BIND(LightSource[0].specular);
    GL_BIND(LightSource[0].constantAttenuation);
    GL_BIND(LightSource[0].linearAttenuation);
    GL_BIND(LightSource[0].quadraticAttenuation);
    GL_BIND(LightSource[0].spotDirection);
    GL_BIND(LightSource[0].spotExponent);
    GL_BIND(LightSource[0].spotCutoff);
    GL_BIND(LightSource[0].spotCosCutoff);
#undef GL_BIND

    glUseProgram(p);

    /* Default light source */
    glUniform4fv(world.R.LightSource[0].ambient, 1, world.lights[0].ambient);
    glUniform4fv(world.R.LightSource[0].diffuse, 1, world.lights[0].diffuse);
    glUniform4fv(world.R.LightSource[0].specular, 1, world.lights[0].specular);
    glUniform4fv(world.R.LightSource[0].position, 1, world.lights[0].position);
    glUniform1f(world.R.LightSource[0].spotExponent, world.lights[0].spotExponent);
    glUniform1f(world.R.LightSource[0].constantAttenuation, world.lights[0].constantAttenuation);
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

