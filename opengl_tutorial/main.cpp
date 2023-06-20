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

unsigned int constructShader()
{
    
    unsigned int vertexShaderId;
    unsigned int fragmentShaderId;

    vertexShaderId = glCreateShader( GL_VERTEX_SHADER );
    fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );

    glShaderSource( vertexShaderId, 1, &vertexShaderSource, NULL );
    glCompileShader( vertexShaderId );

#ifdef DEBUG_SHADER
    int success;
    char infoLog[ 512 ];
    glGetShaderiv( vertexShaderId, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        glGetShaderInfoLog( vertexShaderId, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
    }
#endif // DEBUG_SHADER

    glShaderSource( fragmentShaderId, 1, &fragmentShaderSource, NULL );
    glCompileShader( fragmentShaderId );

#ifdef DEBUG_SHADER
    glGetShaderiv( fragmentShaderId, GL_COMPILE_STATUS, &success );

    if ( !success )
    {
        glGetShaderInfoLog( fragmentShaderId, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
    }
#endif // DEBUG_SHADER

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader( shaderProgram, vertexShaderId );
    glAttachShader( shaderProgram, fragmentShaderId );
    glLinkProgram( shaderProgram );

#ifdef DEBUG_SHADER
    glGetShaderiv( shaderProgram, GL_LINK_STATUS, &success );

    if ( !success )
    {
        glGetShaderInfoLog( fragmentShaderId, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
#endif // DEBUG_SHADER

    // clean up
    glDeleteShader( vertexShaderId );
    glDeleteShader( fragmentShaderId );

    return shaderProgram;
}

void constructTriangle(float* vertices, size_t size, unsigned int &vao, unsigned int &vbo )
{
    // unsigned int vao;
    // unsigned int vbo;

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    // bind vertex array object
    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( *vertices ) * size , vertices, GL_STATIC_DRAW );

    // link vertex attributes
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *) 0 );
    glEnableVertexAttribArray( 0 );

    // glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    // return vao;

}

void draw( unsigned int vao, unsigned int shaderProgram)
{

    // glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glUseProgram( shaderProgram );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, 3 );
    
    glBindVertexArray( 0 );
    glUseProgram( 0 );

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

    // calls this before any gl calls
    gladLoadGL();

    if ( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) )
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport( 0, 0, 800, 600 );
    glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

    unsigned int shaderProgram = constructShader();
    glUseProgram( shaderProgram );

    // unsigned int vao = constructTriangle( vertices );
    unsigned int vao, vbo;
    constructTriangle( vertices, sizeof( vertices ) / sizeof( *vertices ), vao, vbo );
    

    // glGenVertexArrays( 1, &vao );
    // glGenBuffers( 1, &vbo );

    // bind vertex array object
    glBindVertexArray( vao );
    
    // glBindBuffer( GL_ARRAY_BUFFER, vbo );
    // glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // link vertex attributes
    // glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *) 0 );
    // glEnableVertexAttribArray( 0 );

    // glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    
    glUseProgram( 0 );

    while( !glfwWindowShouldClose( window ) )
    {
        processInput( window );
        glClear( GL_COLOR_BUFFER_BIT );

        draw( vao, shaderProgram );

        glfwSwapBuffers( window );
        glfwPollEvents();    
    }

    glfwTerminate();

    return 0;
}