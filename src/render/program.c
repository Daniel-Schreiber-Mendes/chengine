#include "../../engine.h"

static GLuint createShader(GLenum const type, char const *const path);

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
    glDetachShader(*program, vs);
    glDetachShader(*program, fs);

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
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
	glUniform4f(location, v0, v1, v2, v2);
}


void program_uniform1i_set(GLuint const program, char const *const name, GLint const v0)
{
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1i(location, v0);
}


void program_uniformMat4f_set(GLuint const program, char const *const name, mat4 const m0)
{
    // GL_FALSE because layout is already row major
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniformMatrix4fv(location, 1, GL_FALSE, m0[0]);
}


static GLuint createShader(GLenum const type, char const *const path)
{
    GLuint const shader = glCreateShader(type);
    {
        File file;
        file_load(&file, path);
        glShaderSource(shader, 1, (char const *const *)&file.buffer, NULL);
        file_destruct(&file);
    }
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        
        char *const message = malloc(sizeof(char) * len);
        glGetShaderInfoLog(shader, len, &len, message);
        printf("Failed to compiler shader!\n\nPath: \n%s\n\nMessage:\n%s", path, message);
        free(message);
    }

    return shader;
}