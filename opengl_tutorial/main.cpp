#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

#define DEBUG_SHADER

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

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

const char* vertexShaderSource = R"""(
#version 420 core
layout ( location = 0 ) in vec3 aPos;
void main()
{
    gl_Position = vec4( aPos.x, aPos.y, aPos.z, 1.0 );
}
)""";

const char* fragmentShaderSource = R"""(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)""";

void constructTriangle(float* vertices )
{
    unsigned int VBO;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    glGenBuffers( 1, &VBO );
    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( *vertices ), vertices, GL_STATIC_DRAW );

    glShaderSource( vertexShader, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShader );

#ifdef DEBUG_SHADER
    int success;
    char infoLog[ 512 ];
    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        glGetShaderInfoLog( vertexShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
    }
#endif // DEBUG_SHADER

    glShaderSource( fragmentShader, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShader );

#ifdef DEBUG_SHADER
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        glGetShaderInfoLog( fragmentShader, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
    }
#endif // DEBUG_SHADER

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glLinkProgram( shaderProgram );
    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

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
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    constructTriangle( vertices );

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );
        glfwPollEvents();    

        glClear( GL_COLOR_BUFFER_BIT );
        glfwSwapBuffers( window );
    }

    glfwTerminate();

    return 0;
}