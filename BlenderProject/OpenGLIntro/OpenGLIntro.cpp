#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Vertex Shader
const char *vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;  // Vertex position

    uniform mat4 transform;

    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view *transform* vec4(aPos, 1.0);
    }
)glsl";

// Fragment Shader
const char *fragmentShaderSource = R"glsl(
        #version 330 core
        out vec4 color;
        void main() {
            color = vec4(1.0, 1.0, 1.0, 1.0); //White Color
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

// Struct to store OBJ data
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
};
/**
 * @brief Sets up the Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO) for the triangle.
 */
void setupBuffers(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    // Setup the VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO);

    // Bind the VAO for the object
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
void processInput(GLFWwindow *window, float &xOffset, float &yOffset, float &scale, float &rotationX, float &rotationY, float &rotationZ)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        yOffset += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        yOffset -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        xOffset -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        xOffset += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        xOffset -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        xOffset += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        rotationX += 2.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        rotationX -= 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotationY += 2.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotationY -= 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        rotationZ += 2.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        rotationZ -= 2.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        scale += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        scale -= 0.01f;
    }

    // Ensure scale does not go below a minimum value
    if (scale < 0.1f)
    {
        scale = 0.1f;
    }
}

// Function to load OBJ file
bool loadOBJ(const std::string &path, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }
        else if (prefix == "vt")
        {
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "vn")
        {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "f")
        {
            std::string vertexData;
            unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];
            for (int i = 0; i < 3; ++i)
            {
                iss >> vertexData;

                // Use sscanf_s for secure string parsing if you're on Windows (MSVC).
                // For cross-platform code, use the safer sscanf version.
                int matches = sscanf_s(vertexData.c_str(), "%d/%d/%d", &vertexIndex[i], &texCoordIndex[i], &normalIndex[i]);

                // If you want to keep it platform independent, you can still use sscanf but check the return value.
                // int matches = sscanf(vertexData.c_str(), "%d/%d/%d", &vertexIndex[i], &texCoordIndex[i], &normalIndex[i]);

                // if (matches != 3) {
                //     std::cerr << "Error parsing OBJ file: " << path << std::endl;
                //     return false;
                // }

                Vertex vertex;
                vertex.Position = tempPositions[vertexIndex[i] - 1];
                vertex.Normal = tempNormals[normalIndex[i] - 1];
                outVertices.push_back(vertex);
                outIndices.push_back(static_cast<unsigned int>(outVertices.size() - 1));
            }
        }
    }

    return true;
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "3D Model Loader", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, 800, 600);

    compileShaders();

    // Load the model
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    if (!loadOBJ("bottle.obj", vertices, indices))
    {
        return -1;
    }

    // set object mode to wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    setupBuffers(vertices, indices);
    glUseProgram(shaderProgram);
    int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float scale = 1.0f;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        // Update transformation values
        processInput(window, xOffset, yOffset, scale, rotationX, rotationY, rotationX);
        // print the values
        std::cout << "xOffset: " << xOffset << " yOffset: " << yOffset << " scale: " << scale << " rotationX: " << rotationX << " rotationY: " << rotationY << " rotationZ: " << rotationZ << std::endl;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transform = glm::mat4(1.0f);

        // Apply translation
        transform = glm::translate(transform, glm::vec3(xOffset, yOffset, 0.0f));

        // Apply rotation
        transform = glm::rotate(transform, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));


        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        
        // Apply scaling
        transform = glm::scale(transform, glm::vec3(scale, scale, scale));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Render the loaded model
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
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