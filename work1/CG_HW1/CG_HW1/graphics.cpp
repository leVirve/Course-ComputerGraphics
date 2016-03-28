#include "graphics.h"


void loadOBJModel(int index)
{
	if (model != NULL) {
		free(model);
	}
	char* filename = (char*) modelfiles[index].c_str();
	char title[1024];

	model = glmReadOBJ((char*) filename);
	printf("%s\n", filename);

	snprintf(title, 1024, "%s - 10420 CS550000 CG HW1 Salas", filename);
	glutSetWindowTitle(title);

	normalizeModel();
}


void normalizeModel()
{
	GLfloat scale = 0;

	for (int k = 0; k < 3; ++k) {
		GLfloat max = -FLT_MAX, min = FLT_MAX, centroid, len;

		for (unsigned int i = 1; i <= model->numvertices; ++i) {
			GLfloat val = model->vertices[3 * i + k];
			if (val > max) max = val;
			if (min > val) min = val;
		}

		centroid = (max + min) / 2;
		len = (max - min) / 2;
		if (len > scale) scale = len;

		for (unsigned int i = 1; i <= model->numvertices; ++i)
			model->vertices[3 * i + k] -= centroid;
	}

	for (int k = 0; k < 3; ++k)
		for (unsigned int i = 0; i <= model->numvertices; ++i)
			model->vertices[3 * i + k] /= scale;
}

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

	glUseProgram(p);
}
