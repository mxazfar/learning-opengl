// Interfaces used
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_manager.h"
#include "utility.h"

// Standard library includes
#include <math.h>
#include <stdio.h>

#define DEFAULT_WIDTH	800U
#define DEFAULT_HEIGHT	600U

/******************************************************************************
 *                                  TYPEDEFS                                 *
 *****************************************************************************/

typedef struct
{ 
    GLFWwindow* window;

    unsigned int vertex_buffer_object;
    unsigned int vertex_array_handle;
    shader_program_s shader_program;
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

    glUseProgram(em.shader_program.handle);

    float time = glfwGetTime();
    float redValue = (cos(2*time) / 3.0f) + 0.5f;
    float greenValue = (sin(time) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(em.shader_program.handle, "ourColor");

    glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);

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

    em.shader_program = SHADER_linkShaderProgram("simple program", "shaders/simple_vertex_shader.glsl", "shaders/simple_fragment_shader.glsl");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(em.window))
    {
	renderLoop();
    }

    glfwTerminate();
    return 0;
}

