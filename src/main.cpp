#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using namespace std;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n" //color
"}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Lab1Logo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 800, 800);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.2f, -0.5f, 0.0f,
        -0.2f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.2f, -0.5f, 0.0f,
        0.2f, 0.5f, 0.0f,
        -0.2f, 0.0f, 0.0f,
        0.2f, 0.0f, 0.0f,
		-0.948f, -0.2f, 0.0f,
		0.947f, -0.2f, 0.0f,
		-0.948f, 0.1f, 0.0f,
		0.947f, 0.1f, 0.0f,
    };

    GLuint indices[] =
    {
        0, 1, 2, 3,
        4, 5, 6, 7,
        1, 3, 6, 
        6, 3, 4,
		10, 11, 12,
		12, 11, 13
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Circle details for the ring
	const int numSegments = 100;  // Total segments for the ring
	const float radiusOuter = 0.95f;
	const float radiusInner = 0.75f;
	const float startAngle = M_PI / 4;  // Start at 45 degrees
	const float endAngle = M_PI; // End at 135 degrees

	// Calculate the number of segments required for the arc between start 	and end angles
	GLfloat circleVertices[6 * (numSegments + 1)];

	int vertexIndex = 0;
	for (int i = 0; i <= numSegments; ++i) {
	    float angle = startAngle + (endAngle - startAngle) * i / numSegments;
	
	    // Calculate outer ring position
	    float outerX = radiusOuter * cos(angle);
	    float outerY = radiusOuter * sin(angle);
	
	    // Calculate inner ring position
	    float innerX = radiusInner * cos(angle);
	    float innerY = radiusInner * sin(angle);

	    // Outer circle vertex
	    circleVertices[6 * vertexIndex] = outerX;
	    circleVertices[6 * vertexIndex + 1] = outerY;
	    circleVertices[6 * vertexIndex + 2] = 0.0f;

	    // Inner circle vertex
	    circleVertices[6 * vertexIndex + 3] = innerX;
	    circleVertices[6 * vertexIndex + 4] = innerY;
	    circleVertices[6 * vertexIndex + 5] = 0.0f;

	    vertexIndex++;
	}



GLuint circleVAO, circleVBO;
glGenVertexArrays(1, &circleVAO);
glGenBuffers(1, &circleVBO);

glBindVertexArray(circleVAO);
glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glBindVertexArray(0);


GLfloat flippedCircleVertices[6 * (numSegments + 1)];
vertexIndex = 0;
for (int i = 0; i <= numSegments; ++i) {
    float angle = startAngle + (endAngle - startAngle) * i / numSegments;

    // Calculate outer ring position
    float outerX = radiusOuter * cos(angle);
    float outerY = radiusOuter * sin(angle);

    // Calculate inner ring position
    float innerX = radiusInner * cos(angle);
    float innerY = radiusInner * sin(angle);

    // Flip both X and Y coordinates for the second half-ring
    outerX = -outerX; // Flip horizontally
    outerY = -outerY; // Flip vertically
    innerX = -innerX; // Flip horizontally
    innerY = -innerY; // Flip vertically


    // Outer circle vertex (flipped)
    flippedCircleVertices[6 * vertexIndex] = outerX;
    flippedCircleVertices[6 * vertexIndex + 1] = outerY;
    flippedCircleVertices[6 * vertexIndex + 2] = 0.0f;

    // Inner circle vertex (flipped)
    flippedCircleVertices[6 * vertexIndex + 3] = innerX;
    flippedCircleVertices[6 * vertexIndex + 4] = innerY;
    flippedCircleVertices[6 * vertexIndex + 5] = 0.0f;

    vertexIndex++;
}

GLuint flippedCircleVAO, flippedCircleVBO;
glGenVertexArrays(1, &flippedCircleVAO);
glGenBuffers(1, &flippedCircleVBO);

glBindVertexArray(flippedCircleVAO);
glBindBuffer(GL_ARRAY_BUFFER, flippedCircleVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(flippedCircleVertices), flippedCircleVertices, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
glBindVertexArray(0);


// Inside your render loop
while (!glfwWindowShouldClose(window))
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)(4 * sizeof(GLuint)));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(8 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(14 * sizeof(GLuint)));

    // Draw the ring with the calculated vertices
	glBindVertexArray(circleVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (numSegments + 1)); // Adjust as per the segment count

	// Draw the flipped half-ring
    glBindVertexArray(flippedCircleVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (numSegments + 1));

    glfwSwapBuffers(window);
    glfwPollEvents();
}

glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
glDeleteVertexArrays(1, &circleVAO);
glDeleteBuffers(1, &circleVBO);
glDeleteProgram(shaderProgram);
glfwDestroyWindow(window);
glfwTerminate();
}