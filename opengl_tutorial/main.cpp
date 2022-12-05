#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    std::cout << "width " << width << " heigth " << height << std::endl;
    glViewport( 0, 0, width, height );
}


void processInput( GLFWwindow *window )
{
    if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        glfwSetWindowShouldClose( window, true );
}

int main(void)
{
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    GLFWwindow* window = glfwCreateWindow( 800, 600, "first GL window", NULL, NULL );

    if( window == NULL )
    {
        std::cout << "fail to initialize window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent( window );
    gladLoadGL();

    if ( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport( 0, 0, 800, 600 );
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );
        glfwPollEvents();    

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers( window );
    }

    glfwTerminate();

    return 0;
}