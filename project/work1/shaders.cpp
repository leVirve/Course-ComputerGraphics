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

    iLocPosition = glGetAttribLocation(p, "av4position");
    iLocColor = glGetAttribLocation(p, "av3color");
    iLocMVP = glGetUniformLocation(p, "mvp");

    glUseProgram(p);
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

