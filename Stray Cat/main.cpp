//
//  main.cpp
//  Stray Cat
//
//  Created by Alexander Makhov on 18.02.2021.
//

#include "common.h"
#include "Image.h"
#include "Player.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

constexpr GLsizei WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;

struct InputState
{
    bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_1:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case GLFW_KEY_2:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            if (action == GLFW_PRESS)
                Input.keys[key] = true;
            else if (action == GLFW_RELEASE)
                Input.keys[key] = false;
    }
}

void processPlayerMovement(Player &player, std::string &room)
{
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(MovementDir::UP, room);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(MovementDir::DOWN, room);
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(MovementDir::LEFT, room);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(MovementDir::RIGHT, room);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input.captureMouse = !Input.captureMouse;
    
    if (Input.captureMouse)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    if (Input.firstMouse)
    {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }
    
    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);
    
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
        // ...
}


int initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "W, A, S, D - movement  "<< std::endl;
    std::cout << "press ESC to exit" << std::endl;
    
    return 0;
}

std::string ReadAndShowRoom(Image &screen, const char * path) {
    std::ifstream in(path);
    std::string room;
    
    if (in.is_open()) {
        char tile;
        while (in.get(tile)) {
            if (tile != '\n') {
                room.push_back(tile);
            }
        }
    }
    in.close();
    
    Image bush("../../Stray Cat/resources/bush.png");
    Image carrot("../../Stray Cat/resources/carrot.png");
    Image pit("../../Stray Cat/resources/pit.png");
    Image portal_down("../../Stray Cat/resources/portal_down.png");
    Image portal_up("../../Stray Cat/resources/portal_up.png");
    Image portal_right("../../Stray Cat/resources/portal_right.png");
    Image portal_left("../../Stray Cat/resources/portal_left.png");
    
    int screen_x;
    int screen_y;
    
    for (int i = 0; i < room.size(); i++) {
        switch (room[i]) {
            case '#':
                for (int x = 0; x < tileSize; x++) {
                    for (int y = 0; y < tileSize; y++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), bush.GetPixel(x, tileSize - y - 1)));
                    }
                }
                break;
            case ' ':
                for (int x = 0; x < tileSize; x++) {
                    for (int y = 0; y < tileSize; y++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), pit.GetPixel(x, tileSize - y - 1)));
                    }
                }
                break;
            case 'G':
                for (int x = 0; x < tileSize; x++) {
                    for (int y = 0; y < tileSize; y++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), carrot.GetPixel(x, tileSize - y - 1)));
                    }
                }
                break;
            case 'X':
                if ((room[i + 1] == 'X') && (i < 16)) {
                    for (int x = 0; x < tileSize * 2; x++) {
                        for (int y = 0; y < tileSize; y++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_up.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 1] == 'X') && (i >= 16)) {
                    for (int x = 0; x < tileSize * 2; x++) {
                        for (int y = 0; y < tileSize; y++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_down.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 16] == 'X') && (i % 16 < 8)) {
                    for (int x = 0; x < tileSize; x++) {
                        for (int y = 0; y < tileSize * 2; y++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (14 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_left.GetPixel(x, tileSize * 2 - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 16] == 'X') && (i % 16 >= 8)) {
                    for (int x = 0; x < tileSize; x++) {
                        for (int y = 0; y < tileSize * 2; y++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (14 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_right.GetPixel(x, tileSize * 2 - y - 1)));
                        }
                    }
                    break;
                }
                break;
            default:
                break;
        }
    }
    
    return room;
}

int main(int argc, char** argv)
{
    if(!glfwInit())
        return -1;
    
        //    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback        (window, OnKeyboardPressed);
    glfwSetCursorPosCallback  (window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback     (window, OnMouseScroll);
    
    if(initGL() != 0)
        return -1;
    
        //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
        gl_error = glGetError();
    
    Point starting_pos{.x = tileSize, .y = WINDOW_HEIGHT / 2 - tileSize * 3/4};
    Player player{starting_pos};
    
    Image img("../../Stray Cat/resources/grass-background.png");
    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
    
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        for (int j = 0; j < WINDOW_HEIGHT; j++) {
            screenBuffer.PutPixel(i, j, img.GetPixel(i, WINDOW_HEIGHT - j - 1));
        }
    }
    
    std::string currentRoom = ReadAndShowRoom(screenBuffer, "../../Stray Cat/rooms/E.txt");
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
    
        //game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        
        processPlayerMovement(player, currentRoom);
        player.Draw(screenBuffer);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
