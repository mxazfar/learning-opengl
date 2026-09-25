// Interfaces implemented
#include "shader_manager.h"

// Interfaces used
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INFO_LOG_SIZE 	512U

static const char *shaderEnumToStr[SHADER_TYPE_NUM] = {
    [SHADER_TYPE_VERTEX] = "VERTEX",
    [SHADER_TYPE_FRAGMENT] = "FRAGMENT"
};

static char* readShaderFile(const char* path)
{
    if (path == NULL)
    {
        return NULL;
    }

    FILE* file = fopen(path, "rb");

    if (!file)
    {
        fprintf(stdout, "Failed to open shader file at %s\n", path);
	return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

static unsigned int compileShader(const char* source_path, shader_type_e shader_type)
{
    unsigned int handle;

    if (shader_type == SHADER_TYPE_VERTEX)
    {
        handle = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (shader_type == SHADER_TYPE_FRAGMENT)
    {
	handle = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
	// shouldn't enter here
        handle = 0U;
	return handle;
    }

    char* shaderSource = readShaderFile(source_path);

    if (shaderSource == NULL)
    {
        handle = 0U;
	return handle;
    }

    glShaderSource(handle, 1, (const char* const*)&shaderSource, NULL);
    glCompileShader(handle);

    int init_success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &init_success);

    // check that shader compiled successfully
    if(!init_success)
    {
	char infoLog[INFO_LOG_SIZE];
        glGetShaderInfoLog(handle, INFO_LOG_SIZE, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::%s::COMPILATION_FAILED\n %s\n", shaderEnumToStr[shader_type], infoLog);
    }

    return handle;
}

shader_program_s SHADER_linkShaderProgram(const char* name, const char* vertex_path, const char* frag_path)
{
    shader_program_s program;

    unsigned int vertex_shader_handle = compileShader(vertex_path, SHADER_TYPE_VERTEX);
    unsigned int fragment_shader_handle = compileShader(frag_path, SHADER_TYPE_FRAGMENT);

    if ((vertex_shader_handle == 0U) || (fragment_shader_handle == 0U))
    {
        program.handle = 0U;
	char* failName = "failed";
	strcpy(program.name, failName);

	return program;
    }

    // create shader program, link our two shaders to it
    program.handle = glCreateProgram();
    strcpy(program.name, name);

    glAttachShader(program.handle, vertex_shader_handle);
    glAttachShader(program.handle, fragment_shader_handle);
    glLinkProgram(program.handle);

    // shaders handles can be deleted now since they are baked into the program
    glDeleteShader(vertex_shader_handle);
    glDeleteShader(fragment_shader_handle);

    return program;
}

