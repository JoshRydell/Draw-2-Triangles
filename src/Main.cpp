#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;
    const char *fragYShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f, 0.8f, 0.2f, 1.0f);\n"
    "}\0";

    const char *fragOShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void frame_buffer_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0, width,height);
}

unsigned int createVertexShader()
{
    //VERTEX SHADER
    int  success;
    char infoLog[512];
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return vertexShader;
}

unsigned int createFragShader(bool orange)
{
    //FRAGMENT SHADER
    int  success;
    char infoLog[512];


    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader,1, orange ? &fragOShaderSource : &fragYShaderSource,NULL);
    glCompileShader(fragShader);


    glGetShaderiv(fragShader,GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return fragShader;
}

unsigned int createShaderProgram(bool orange)
{
    //SHADER PROGRAM;
    int  success;
    char infoLog[512];
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    unsigned int vertexShader = createVertexShader();
    unsigned int fragShader = createFragShader(orange);

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return shaderProgram;
}

unsigned int createVBO(float vertices[], unsigned int sizeVertices)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeVertices,vertices,GL_STATIC_DRAW);

    return VBO;
}

unsigned int createVAO(float vertices[],unsigned int sizeVertices)//, unsigned int indices[], unsigned int sizeIndices)
{
    unsigned int VBO, VAO;//, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeIndices,indices,GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    return VAO;
}

GLFWwindow *createWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height,"Triangles", NULL, NULL);
    
    if(window == NULL)
    {
        cout << "Window intialisation failed \n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "loading GLAD failed \n";
        glfwTerminate();
    }
    
    glViewport(0,0,width,height);
    glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
    

    return window;

}

int main()
{
    GLFWwindow *window = createWindow(800,600);


    //PROGRAM
    float vertices[2][9] = {
     {0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f},
    {0.5f,  0.5f, 0.0f,  // top right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f }  // bottom left
    };
//of(float), void*

    /*unsigned int indices[] = {  // note that we start from 0!
       0, 1, 3,   // first triangle
       1, 2, 3    // second triangle
    }; */
    const unsigned int shaderProgramLength = 2;
    const unsigned int VAOsLength = 2;
    unsigned int shaderProgram[VAOsLength];
    shaderProgram[0] = createShaderProgram(false);
    shaderProgram[1]= createShaderProgram(true);
    //unsigned int VBO = createVBO(vertices,sizeof(vertices));
    unsigned int VAOs[VAOsLength];
    unsigned int VBOs[VAOsLength];


    glGenVertexArrays(VAOsLength,VAOs);
    glGenBuffers(VAOsLength,VBOs);
    
    

    for(int i = 0; i < VAOsLength; i++)
    {
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_TRUE,3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //WORKING VERSION STOP UNDOING;
    while(!glfwWindowShouldClose(window))
    {

        for(int i = 0; i < VAOsLength; i++)
        {
            glUseProgram(shaderProgram[i]);
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLES,0,3);
        }
        //glDrawElements(GL_TRIANGLES,sizeof(indices),GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    for(int i = 0; i < shaderProgramLength;i++)
    {
        glDeleteProgram(shaderProgram[i]);
    }
    glfwTerminate();
    return 0;
}