// Interfaces used
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library includes
#include <stdio.h>

/******************************************************************************
 *                                  TYPEDEFS                                 *
 *****************************************************************************/

typedef struct
{ 
    GLFWwindow* window;
} engine_manager_s;

/******************************************************************************
 *                               PRIVATE DATA                                *
 *****************************************************************************/

static engine_manager_s em;

/******************************************************************************
 *                       PRIVATE FUNCTION DEFINITIONS                        *
 *****************************************************************************/

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1; 
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for macOS

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(em.window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(em.window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

