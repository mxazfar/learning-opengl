// Interfaces used
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <simple_fragment_shader.h>
#include <simple_vertex_shader.h>
#include "utility.h"

// Standard library includes
#include <stdio.h>


#define DEFAULT_WIDTH	800U
#define DEFAULT_HEIGHT	600U

/******************************************************************************
 *                                  TYPEDEFS                                 *
 *****************************************************************************/

typedef struct
{
    unsigned int handle;
    int init_success;
    char infoLog[512];
} shader_s;

typedef struct
{ 
    GLFWwindow* window;

    unsigned int vertex_buffer_object;

    shader_s vertex_shader;
    shader_s fragment_shader;

    unsigned int shader_program_handle;
    unsigned int vertex_array_handle;
} engine_manager_s;

/******************************************************************************
 *                               PRIVATE DATA                                *
 *****************************************************************************/

static engine_manager_s em;
static float vertices[] = 
{
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};


/******************************************************************************
 *                             PRIVATE FUNCTIONS                             *
 *****************************************************************************/

/*
 * Main rendering loop.
 *
*/
static void renderLoop(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(em.shader_program_handle);
    glBindVertexArray(em.vertex_array_handle);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(em.window);
    glfwPollEvents();
}

/*
 * Sets up GLFW configurations and selecs OpenGL version
*/
static void setupWindowHints(void)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for macOS
}

/*
 * Allocates memory from the GPU to store vertices via VBO
 * Defines the shape of the memory via VAO
*/
static void setupDataMovement(void)
{
    // setup vertex array object
    // this define how the memory allocated in the GPU should be parsed
    glGenVertexArrays(1, &em.vertex_array_handle);
    glBindVertexArray(em.vertex_array_handle);

    // setup vertex buffer object
    // this effectively allocates memory in the GPU
    // this needs to be done after the VAO is created since
    glGenBuffers(1, &em.vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, em.vertex_buffer_object); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    // VBO and VAO must be bound before this is called
    // configures the VAO to look in the VBO for data at:
    // index=0, this must match the location defined for the input in the shader
    // size=3, size of the vetex attribute, 3 for vec3
    // type=data type
    // normalize=false (data is floats)
    // stride=how far apart subseqent verticies are (one vertice is 3 dimensions=3 floats)
    // offset=offset of where the data begins in the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

/*
 * Compiles each shader from its source
 * Creates shader program and links all compiled shaders
*/
static void setupShaders(void)
{
    // setup vertex shader
    em.vertex_shader.handle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(em.vertex_shader.handle, 1, &simpleVertexShaderSource, NULL);
    glCompileShader(em.vertex_shader.handle);
    glGetShaderiv(em.vertex_shader.handle, GL_COMPILE_STATUS, &em.vertex_shader.init_success);

    // check that shader compiled successfully
    if(!em.vertex_shader.init_success)
    {
        glGetShaderInfoLog(em.vertex_shader.handle, 512, NULL, em.vertex_shader.infoLog);
	fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", em.vertex_shader.infoLog);
    }

    // setup fragment shader
    em.fragment_shader.handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(em.fragment_shader.handle, 1, &simpleFragmentShaderSource, NULL);
    glCompileShader(em.fragment_shader.handle);
    glGetShaderiv(em.fragment_shader.handle, GL_COMPILE_STATUS, &em.fragment_shader.init_success);

    // check that fragment shader compiled successfully
    if(!em.fragment_shader.init_success)
    {
        glGetShaderInfoLog(em.fragment_shader.handle, 512, NULL, em.fragment_shader.infoLog);
	fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", em.fragment_shader.infoLog);
    }

    // create shader program, link our two shaders to it, and use the program
    em.shader_program_handle = glCreateProgram();
    glAttachShader(em.shader_program_handle, em.vertex_shader.handle);
    glAttachShader(em.shader_program_handle, em.fragment_shader.handle);
    glLinkProgram(em.shader_program_handle);

    // shaders can be deleted now since they are baked into the program
    glDeleteShader(em.vertex_shader.handle);
    glDeleteShader(em.fragment_shader.handle);
}

/******************************************************************************
 *                             CALLBACK FUNCTIONS                            *
 *****************************************************************************/

static void error_callback(int error, const char* description)
{
    UNUSED(error);
    fprintf(stderr, "Error: %s\n", description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    UNUSED(window);
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    UNUSED(window);
    UNUSED(key);
    UNUSED(scancode);
    UNUSED(action);
    UNUSED(mods);
}

static void registerCallbacks(void)
{
    glfwSetFramebufferSizeCallback(em.window, framebuffer_size_callback);
    glfwSetKeyCallback(em.window, key_callback);
}

/******************************************************************************
 *                           PROGRAM ENTRY POINT                             *
 *****************************************************************************/

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1; 
    }
    
    setupWindowHints();

    /* Create a windowed mode window and its OpenGL context */
    em.window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!em.window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(em.window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;	
    }

    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    registerCallbacks();

    setupDataMovement();

    setupShaders();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(em.window))
    {
	renderLoop();
    }

    glfwTerminate();
    return 0;
}

