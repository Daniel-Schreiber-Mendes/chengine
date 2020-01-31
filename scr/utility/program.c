#include "utility.h"

GLuint createShader(GLenum const type, char const *const path);

void program_create(GLuint *const program, char const *const vsPath, char const *const fsPath)
{
    //it is important that the source is relative to the .o file not relative to the .c file
    *program = glCreateProgram();
    GLuint const vs = createShader(GL_VERTEX_SHADER, vsPath);
    GLuint const fs = createShader(GL_FRAGMENT_SHADER, fsPath);

    glAttachShader(*program, vs);
    glAttachShader(*program, fs);
    glLinkProgram(*program);
    glValidateProgram(*program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    program_bind(*program);
}


void program_destroy(GLuint const program)
{
    glDeleteProgram(program);
}


void program_bind(GLuint const program)
{
	glUseProgram(program);
}


void program_uniform4f_set(GLuint const program, char const *const name, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3)
{
	glUniform4f(glGetUniformLocation(program, name), v0, v1, v2, v2);
}


void program_uniform1i_set(GLuint const program, char const *const name, GLint const v0)
{
    glUniform1i(glGetUniformLocation(program, name), v0);
}


void program_uniformMat4f_set(GLuint const program, char const *const name, mat4 const m0)
{
    // GL_FALSE because layout is already row major
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, m0[0]);
}


GLuint createShader(GLenum const type, char const *const path)
{
    File file;
    file_load(&file, path);

    GLuint const shader = glCreateShader(type);
    glShaderSource(shader, 1, &file.buffer, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

        char *const message = (char*)alloca(sizeof(char) * len); //stack allocating string
        glGetShaderInfoLog(shader, len, &len, message);

        printf("Failed to compiler shader!\n\nPath: \n%s\n\nBuffersize: %u\n\nMessage:\n%s", path, file.bufferSize, message);
        printf("%s\n", file.buffer);
    }

    return shader;
}