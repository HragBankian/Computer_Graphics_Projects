// OpenGLIntro.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>


// Define Vertex and Fragment Shader
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 transform;
    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White color
    }
)glsl";



unsigned int vertexShader, fragmentShader, shaderProgram;

/**
 * @brief Compiles the vertex and fragment shaders and links them into a shader program.
 */
void compileShaders()
{
    // Vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR: vertex compliation failed\n"
            << infoLog << std::endl;
    }

    // Fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR: shader compliation failed\n"
            << infoLog << std::endl;
    }

    // Link shaders into a program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR: shader program linking failed\n"
            << infoLog << std::endl;
    }

    // Clean up shaders as they're linked into our program now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int VBO[2], VAO[2], EBO;

std::vector<float> vertices;
std::vector<unsigned int> indices;
/**
 * @brief Sets up the Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO) for the triangle.
 */
void setupBuffers()
{
    // Setup the VAO, VBO, and EBO
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO for the triangle
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



bool loadOBJ(const char* path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file" << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_texCoords;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string type;
        s >> type;

        if (type == "v") {
            glm::vec3 vertex;
            s >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        else if (type == "vt") {
            glm::vec2 uv;
            s >> uv.x >> uv.y;
            temp_texCoords.push_back(uv);
        }
        else if (type == "vn") {
            glm::vec3 normal;
            s >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (type == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            for (int i = 0; i < 3; i++) {
                s >> vertexIndex[i];
                if (s.peek() == '/') {
                    s.ignore(1);
                    if (s.peek() != '/') {
                        s >> uvIndex[i];
                    }
                    if (s.peek() == '/') {
                        s.ignore(1);
                        s >> normalIndex[i];
                    }
                }
            }
            indices.push_back(vertexIndex[0] - 1);
            indices.push_back(vertexIndex[1] - 1);
            indices.push_back(vertexIndex[2] - 1);
        }
    }

    for (unsigned int i = 0; i < indices.size(); i++) {
        glm::vec3 vertex = temp_vertices[indices[i]];
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
    }

    file.close();
    return true;
}

/**
 * @brief Processes keyboard input to control the transformations of the triangle.
 *
 * @param window The GLFW window.
 * @param xOffset The x-axis offset for translation.
 * @param yOffset The y-axis offset for translation.
 * @param angle The angle for rotation.
 * @param scale The scale factor.
 */
void processInput(GLFWwindow* window, float& xOffset, float& yOffset, float& angle, float& scale)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        yOffset += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        yOffset -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        xOffset -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        xOffset += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        angle += 30;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        angle -= 30;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        scale += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        scale -= 0.01f;
    }

    // Ensure scale does not go below a minimum value
    if (scale < 0.1f) {
        scale = 0.1f;
    }
}

/**
 * @brief Main function where program execution begins.
 *
 * @return int Returns 0 if the program executes successfully, otherwise returns -1.
 */
int main()
{
    
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Load the OBJ file
    if (!loadOBJ("bottle.obj")) {
        std::cerr << "Object not found";
        return -1;
    }

    //Polygon mode makes it wareframe looking
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Setup Buffers
    compileShaders();
    setupBuffers();

    glUseProgram(shaderProgram);
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float angle = 0.0f;
    float scale = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        // Render commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update transformation values
        processInput(window, xOffset, yOffset, angle, scale);

        glm::mat4 transform = glm::mat4(1.0f);

        // Apply translation
        transform = glm::translate(transform, glm::vec3(xOffset, yOffset, 0.0f));

        // Apply rotation
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        // Apply scaling
        transform = glm::scale(transform, glm::vec3(scale, scale, scale));

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
         glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
