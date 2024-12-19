#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White color
    }
)glsl";

// Midpoint Circle Drawing Algorithm
std::vector<float> generateCircleVertices(float centerX, float centerY, float radius) {
    std::vector<float> vertices;

    float scaledRadius = radius; // Ensure the radius fits within [-1, 1]
    int x = 0;
    int y = static_cast<int>(scaledRadius * 800); // Scale to screen resolution
    int p = 1 - y;

    auto plotPoints = [&](int x, int y) {
        float fx = static_cast<float>(x) / 800.0f; // Normalize to [-1, 1]
        float fy = static_cast<float>(y) / 800.0f;
        vertices.push_back(centerX + fx);
        vertices.push_back(centerY + fy);
        vertices.push_back(centerX - fx);
        vertices.push_back(centerY + fy);
        vertices.push_back(centerX + fx);
        vertices.push_back(centerY - fy);
        vertices.push_back(centerX - fx);
        vertices.push_back(centerY - fy);
        vertices.push_back(centerX + fy);
        vertices.push_back(centerY + fx);
        vertices.push_back(centerX - fy);
        vertices.push_back(centerY + fx);
        vertices.push_back(centerX + fy);
        vertices.push_back(centerY - fx);
        vertices.push_back(centerX - fy);
        vertices.push_back(centerY - fx);
    };

    plotPoints(x, y);

    while (x < y) {
        ++x;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            --y;
            p += 2 * (x - y) + 1;
        }
        plotPoints(x, y);
    }

    return vertices;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Midpoint Circle Drawing", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);

    // Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Link Shaders into a Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate Circle Vertices
    float centerX = 0.0f, centerY = 0.0f, radius = 0.5f; // Circle in NDC
    std::vector<float> circleVertices = generateCircleVertices(centerX, centerY, radius);

    // Create VAO and VBO for Circle
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, circleVertices.size() / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
