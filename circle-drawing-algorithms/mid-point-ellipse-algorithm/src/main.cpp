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

// Midpoint Ellipse Drawing Algorithm
std::vector<float> generateEllipseVertices(float centerX, float centerY, float rx, float ry) {
    std::vector<float> vertices;

    float x = 0.0f;
    float y = ry;

    float rxSq = rx * rx;
    float rySq = ry * ry;

    // Initial decision parameter for Region 1
    float p1 = rySq - (rxSq * ry) + (0.25f * rxSq);

    auto plotPoints = [&](float x, float y) {
        vertices.push_back(centerX + x);
        vertices.push_back(centerY + y);
        vertices.push_back(centerX - x);
        vertices.push_back(centerY + y);
        vertices.push_back(centerX + x);
        vertices.push_back(centerY - y);
        vertices.push_back(centerX - x);
        vertices.push_back(centerY - y);
    };

    // Region 1
    while ((2 * rySq * x) < (2 * rxSq * y)) {
        plotPoints(x / 800.0f, y / 800.0f); // Normalize points for OpenGL
        x++;
        if (p1 < 0) {
            p1 += (2 * rySq * x) + rySq;
        } else {
            y--;
            p1 += (2 * rySq * x) - (2 * rxSq * y) + rySq;
        }
    }

    // Initial decision parameter for Region 2
    float p2 = rySq * (x + 0.5f) * (x + 0.5f) + rxSq * (y - 1) * (y - 1) - rxSq * rySq;

    // Region 2
    while (y >= 0) {
        plotPoints(x / 800.0f, y / 800.0f); // Normalize points for OpenGL
        y--;
        if (p2 > 0) {
            p2 += rxSq - (2 * rxSq * y);
        } else {
            x++;
            p2 += (2 * rySq * x) - (2 * rxSq * y) + rxSq;
        }
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

    GLFWwindow* window = glfwCreateWindow(800, 800, "Midpoint Ellipse Drawing", nullptr, nullptr);
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

    // Generate Ellipse Vertices
    float centerX = 0.0f, centerY = 0.0f;
    float rx = 400.0f, ry = 300.0f; // Radii of the ellipse (in pixels)
    std::vector<float> ellipseVertices = generateEllipseVertices(centerX, centerY, rx, ry);

    // Create VAO and VBO for Ellipse
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, ellipseVertices.size() * sizeof(float), ellipseVertices.data(), GL_STATIC_DRAW);

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
        glDrawArrays(GL_POINTS, 0, ellipseVertices.size() / 2);

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
