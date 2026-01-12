#include "gl.h"

static GLuint createShader(GLenum const type, char const *const path);

void program_construct(uint32_t *const program, char const *const vsPath, char const *const fsPath)
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

    glUseProgram(*program);
}

void program_uniform4f_set(uint32_t const program, char const *const name, GLfloat const v0, GLfloat const v1, GLfloat const v2, GLfloat const v3)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
	glUniform4f(location, v0, v1, v2, v3);
}


void program_uniform1i_set(uint32_t const program, char const *const name, GLint const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1i(location, v0);
}


void program_uniformMat4_set(uint32_t const program, char const *const name, mat4 const m0)
{
    // GL_FALSE because layout is already row major
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniformMatrix4fv(location, 1, GL_FALSE, m0[0]);
}


void program_uniform4fv_set(uint32_t const program, char const *const name, vec4 const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform4fv(location, 1, v0);
}


void program_uniform1f_set(uint32_t const program, char const *const name, float const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1f(location, v0);
}


void program_uniform1u_set(uint32_t const program, char const *const name, uint32_t const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1ui(location, v0);
}


void program_uniform1uv_set(uint32_t const program, char const *const name, uint16_t const size, uint32_t const *const v0)
{
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1uiv(location, size, v0); 
}


void program_uniform2uv_set(uint32_t const program, char const *const name, uint16_t const size, uint32_t const *const v0)
{
    glUseProgram(program);
    GLint const location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform2uiv(location, size, v0);
}


void program_uniform2fv_set(uint32_t const program, char const *const name, uint16_t const size, vec2 const v0)
{
    glUseProgram(program);
    GLint const location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform2fv(location, size, v0);
}


void program_uniform1fv_set(uint32_t const program, char const *const name, uint16_t const size, float const *const v0)
{
    glUseProgram(program);
    GLint const location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniform1fv(location, size, v0);
}


void program_uniformMat4v_set(uint32_t program, char const *name, mat4 const *mv, uint16_t size)
{
    glUseProgram(program);
    GLint const location = glGetUniformLocation(program, name);
    assert(location != -1);
    glUniformMatrix4fv(location, size, GL_FALSE, mv);
}


static char* fbcp(FILE *const f)
{

    fseek(f, 0, SEEK_END); //move file pointer to end of file
    int64_t const buffersize = ftell(f); //tell position of pointer which means length of string
    fseek(f, 0, SEEK_SET); //move file pointer back to file begin

    char *const buffer = malloc(buffersize + 1);
    fread(buffer, buffersize, 1, f); //copy memory in file to buffer
    buffer[buffersize] = '\0'; //the string has to end with a null terminator, so put it at the end of it
    return buffer;
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
        char const *const buffer = fbcp(file); //don't allocate on stack cause it may cause an overflow if too big
        glShaderSource(shader, 1, &buffer, NULL);
        free((void*)buffer);
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