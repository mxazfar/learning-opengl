#pragma once

typedef enum
{
    SHADER_TYPE_VERTEX,
    SHADER_TYPE_FRAGMENT,
    SHADER_TYPE_NUM
} shader_type_e;

typedef struct
{
    unsigned int handle;
    char name[64];
} shader_program_s;

shader_program_s SHADER_linkShaderProgram(const char* name, const char* vertex_path, const char* frag_path);

