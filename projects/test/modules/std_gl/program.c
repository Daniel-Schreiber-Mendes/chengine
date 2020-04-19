#include "gl.h"
#include <cheio/io.h>

static GLuint createShader(GLenum const type, char const *const path);

void program_construct(Program *const program, char const *const vsPath, char const *const fsPath)
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


void program_destruct(Program const program)
{
    glDeleteProgram(program);
}


void program_bind(Program const program)
{
	glUseProgram(program);
}


void program_uniform4f_set(Program const program, char const *const name, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    che_assert(location != -1);
	glUniform4f(location, v0, v1, v2, v3);
}


void program_uniform1i_set(Program const program, char const *const name, GLint const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    che_assert(location != -1);
    glUniform1i(location, v0);
}


void program_uniformMat4_set(Program const program, char const *const name, mat4 const m0)
{
    // GL_FALSE because layout is already row major
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    che_assert(location != -1);
    glUniformMatrix4fv(location, 1, GL_FALSE, m0[0]);
}


void program_uniform4fv_set(Program const program, char const *const name, vec4 const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    che_assert(location != -1);
    glUniform4fv(location, 1, v0);
}


void program_uniform1f_set(Program const program, char const *const name, float const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    che_assert(location != -1);
    glUniform1f(location, v0);
}


void program_uniform1u_set(Program const program, char const *const name, float const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    che_assert(location != -1);
    glUniform1ui(location, v0);
}



static GLuint createShader(GLenum const type, char const *const path)
{
    GLuint const shader = glCreateShader(type);
    {
        FILE* file = fopen(path, "r");
        if (!file)
        {
            printf("File %s couldn't be opened\n", path);
            return 0;
        }
        char const *const buffer = fbcp(file);
        glShaderSource(shader, 1, (char const *const *)&buffer, NULL);
        free(buffer);
    }
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        
        char *const message = che_scope_malloc(sizeof(char) * len);
        glGetShaderInfoLog(shader, len, &len, message);
        printf("Failed to compiler shader!\n\nPath: \n%s\n\nMessage:\n%s", path, message);
        che_scope_free(message);
    }

    return shader;
}