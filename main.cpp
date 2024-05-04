#include <E:\Projects\Rendering\OpenglEngine\glfw-3.4.bin.WIN32\include\GLFW\glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <windows.h>
#pragma comment(lib, "opengl32.lib")

using namespace std;
int tri = 0;
int mode = 0;
bool canRun = true;


std::mutex consoleMutex;
void handleCommand(const std::string& command) {
    // Acquire the mutex before accessing the console
    std::lock_guard<std::mutex> lock(consoleMutex);

    if (command == "clear") {
        std::cout << "\033[2J\033[H";
    }
    if (command == "close") {
        canRun = false;
    }
    if (command == "2d") {
        mode = 1;
    }
    if (command == "3d") {
        mode = 2;
    }
    if (command == "open") {
        canRun = true;
    }
}

void runCommandPrompt() {
    while (true) {
        std::string userInput;
        cin >> userInput;
        handleCommand(userInput);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE); // Close the window when Escape is pressed
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "Space key pressed\n"; // Write to the console when Space is pressed
    }
}

void gluLookAtManual(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
    GLfloat centerX, GLfloat centerY, GLfloat centerZ,
    GLfloat upX, GLfloat upY, GLfloat upZ) {
    // Calculate the forward vector
    GLfloat forwardX = centerX - eyeX;
    GLfloat forwardY = centerY - eyeY;
    GLfloat forwardZ = centerZ - eyeZ;

    // Normalize the forward vector
    GLfloat forwardLength = sqrt(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    if (forwardLength != 0.0f) {
        forwardX /= forwardLength;
        forwardY /= forwardLength;
        forwardZ /= forwardLength;
    }

    // Calculate the right vector
    GLfloat rightX = forwardY * upZ - forwardZ * upY;
    GLfloat rightY = forwardZ * upX - forwardX * upZ;
    GLfloat rightZ = forwardX * upY - forwardY * upX;

    // Normalize the right vector
    GLfloat rightLength = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
    if (rightLength != 0.0f) {
        rightX /= rightLength;
        rightY /= rightLength;
        rightZ /= rightLength;
    }

    // Calculate the up vector
    GLfloat upLength = sqrt(upX * upX + upY * upY + upZ * upZ);
    if (upLength != 0.0f) {
        upX /= upLength;
        upY /= upLength;
        upZ /= upLength;
    }

    // Set up the view matrix
    GLfloat matrixView[16] = {
        rightX, upX, -forwardX, 0.0f,
        rightY, upY, -forwardY, 0.0f,
        rightZ, upZ, -forwardZ, 0.0f,
        0.0f,    0.0f,    0.0f,   1.0f
    };
    glMultMatrixf(matrixView);
    glTranslatef(-eyeX, -eyeY, -eyeZ);
}

void drawCube() {
    std::vector<GLfloat> vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,  // Bottom-left
         0.5f, -0.5f,  0.5f,  // Bottom-right
         0.5f,  0.5f,  0.5f,  // Top-right
        -0.5f,  0.5f,  0.5f,  // Top-left
        // Back face
        -0.5f, -0.5f, -0.5f,  // Bottom-left
         0.5f, -0.5f, -0.5f,  // Bottom-right
         0.5f,  0.5f, -0.5f,  // Top-right
        -0.5f,  0.5f, -0.5f   // Top-left
    };

    // Define the indices of the cube's faces
    std::vector<GLuint> indices = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Back face
        7, 6, 5,
        5, 4, 7,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Bottom face
        4, 5, 1,
        1, 0, 4,
        // Top face
        3, 2, 6,
        6, 7, 3
    };

    // Define the colors of the vertices
    std::vector<GLfloat> colors = {
        // Front face (red)
        1.0f, 0.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, 0.0f, // Bottom-right
        1.0f, 0.0f, 0.0f, // Top-right
        1.0f, 0.0f, 0.0f, // Top-left
        // Back face (green)
        0.0f, 1.0f, 0.0f, // Bottom-left
        0.0f, 1.0f, 0.0f, // Bottom-right
        0.0f, 1.0f, 0.0f, // Top-right
        0.0f, 1.0f, 0.0f, // Top-left
    };

    // Draw the cube with colors
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glColorPointer(3, GL_FLOAT, 0, colors.data());
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
void drawTriangle1() {
    // Set up the vertices for the triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);   // Red
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 1.0f, 0.0f);   // Green
    glVertex2f(0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f);   // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();
}
void drawTriangle2() {
    // Set up the vertices for the triangle
    glBegin(GL_TRIANGLES);
    glColor3f(2.0f, 0.0f, 1.0f);   // Red
    glVertex2f(-0.5f, -0.5f);
    glColor3f(0.0f, 0.5f, 0.0f);   // Green
    glVertex2f(0.5f, -0.5f);
    glColor3f(1.0f, 0.0f, 0.0f);   // Blue
    glVertex2f(0.0f, 0.5f);
    glEnd();
}

void createWindow() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            std::cout << "2D mod aktivalva\n"; // Write to the console when Space is pressed
            mode = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            std::cout << "3D mod aktivalva\n"; // Write to the console when Space is pressed
            mode = 2;
        }

        if (mode == 2) {
            // Set up the projection matrix
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            float aspectRatio = 800.0f / 600.0f;
            float nearPlane = 0.1f;
            float farPlane = 100.0f;
            float fov = 45.0f;
            float f = 1.0f / tan((fov * 0.5f) * (3.14159265358979323846f / 180.0f));
            float matrixProjection[16] = {
                f / aspectRatio, 0.0f, 0.0f, 0.0f,
                0.0f, f, 0.0f, 0.0f,
                0.0f, 0.0f, (farPlane + nearPlane) / (nearPlane - farPlane), -1.0f,
                0.0f, 0.0f, (2.0f * farPlane * nearPlane) / (nearPlane - farPlane), 0.0f
            };
            glLoadMatrixf(matrixProjection);

            // Set up the view matrix
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAtManual(0.0f, 0.0f, 5.0f,  // Eye position
                0.0f, 0.0f, 0.0f,  // Look-at position
                0.0f, 1.0f, 0.0f); // Up direction

// Rotate the cube
            static float angle = 0.0f;
            angle += 1.0f;
            glRotatef(angle, 1.0f, 1.0f, 1.0f); // Rotate around the diagonal axis

            // Call the function to draw the cube
            drawCube();

        }

        if (mode == 1) {
            // Call the function to draw the triangle
            if (tri == 0) {
                drawTriangle1();
            }
            if (tri == 1) {
                drawTriangle2();
            }

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                std::cout << "Space lenyomva\n"; // Write to the console when Space is pressed
                tri = 1;
            }

            if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
                std::cout << "B lenyomva\n"; // Write to the console when Space is pressed
                tri = 0;
            }
        }




        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            std::cout << "Ablak bezarasa\n"; // Write to the console when Space is pressed
            canRun = false;
        }
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        if (canRun == false) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    std::thread commandPromptThread(runCommandPrompt);
    createWindow();
    commandPromptThread.join();
    runCommandPrompt();
    return 0;
}
