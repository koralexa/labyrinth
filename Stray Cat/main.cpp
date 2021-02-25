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
#include <map>
#include <fstream>

constexpr Pixel brown {218, 165, 32, 1};

constexpr GLsizei WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1088;

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

void processPlayerMovement(Player &player, std::string &room, Image &currentBackground, Image &screen)
{
    if (!player.GetActive()) {
        return;
    }
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(MovementDir::UP, room, currentBackground, screen);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(MovementDir::DOWN, room, currentBackground, screen);
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(MovementDir::LEFT, room, currentBackground, screen);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(MovementDir::RIGHT, room, currentBackground, screen);
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

std::string ReadFileToString(const char * path) {
    std::ifstream in(path);
    std::string s;
    
    if (in.is_open()) {
        char c;
        while (in.get(c)) {
            if (c != '\n') {
                s.push_back(c);
            }
        }
    }
    in.close();
    
    return s;
}

void MakeBackground(std::string &room, Image &screen) {
    Image bush("../../Stray Cat/resources/bush.png");
    Image pit("../../Stray Cat/resources/pit.png");
    Image portal_down("../../Stray Cat/resources/portal_down.png");
    Image portal_up("../../Stray Cat/resources/portal_up.png");
    Image portal_right("../../Stray Cat/resources/portal_right.png");
    Image portal_left("../../Stray Cat/resources/portal_left.png");
    Image finish_portal("../../Stray Cat/resources/finish_portal.png");
    Image grass("../../Stray Cat/resources/grass-background.png");
    
    int screen_x;
    int screen_y;
    
    for (int i = 0; i < grass.Width(); i++) {
        for (int j = 0; j < grass.Height(); j++) {
            screen.PutPixel(i, j, grass.GetPixel(i, grass.Height() - j - 1));
        }
    }
    
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
            case 'Q':
                if (room[i + 1] == 'Q') {
                    for (int x = 0; x < tileSize * 2; x++) {
                        for (int y = 0; y < tileSize; y++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), finish_portal.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                }
                break;
            case 'X':
                if ((room[i + 1] == 'X') && (i / 16 < 8) && (i % 16 != 15)) {
                    for (int x = 0; x < tileSize * 2; x++) {
                        for (int y = 0; y < tileSize; y++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_up.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 1] == 'X') && (i / 16 >= 8) && (i % 16 != 15)) {
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
}

void AddUnstableElements(std::string &room, Image &screen) {
    Image carrot("../../Stray Cat/resources/carrot.png");
    
    int screen_x;
    int screen_y;
    
    for (int i = 0; i < room.size(); i++) {
        switch (room[i]) {
            case 'G':
                for (int x = 0; x < tileSize; x++) {
                    for (int y = 0; y < tileSize; y++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), carrot.GetPixel(x, tileSize - y - 1)));
                    }
                }
                break;
            default:
                break;
        }
    }
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
    
    Point starting_pos{.x = tileSize, .y = WINDOW_HEIGHT / 2 - tileSize * 5/4};
    Player player{starting_pos};
    
    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
    
    std::string plan = ReadFileToString("../../Stray Cat/rooms/plan.txt");
    
    std::map<char, std::string> rooms;
    rooms['A'] = ReadFileToString("../../Stray Cat/rooms/A.txt");
    rooms['B'] = ReadFileToString("../../Stray Cat/rooms/B.txt");
    rooms['C'] = ReadFileToString("../../Stray Cat/rooms/C.txt");
    rooms['D'] = ReadFileToString("../../Stray Cat/rooms/D.txt");
    rooms['E'] = ReadFileToString("../../Stray Cat/rooms/E.txt");
    rooms['F'] = ReadFileToString("../../Stray Cat/rooms/F.txt");
    rooms['G'] = ReadFileToString("../../Stray Cat/rooms/G.txt");
    rooms['H'] = ReadFileToString("../../Stray Cat/rooms/H.txt");
    rooms['I'] = ReadFileToString("../../Stray Cat/rooms/I.txt");
    rooms['J'] = ReadFileToString("../../Stray Cat/rooms/J.txt");
    rooms['K'] = ReadFileToString("../../Stray Cat/rooms/K.txt");
    rooms['L'] = ReadFileToString("../../Stray Cat/rooms/L.txt");
    rooms['M'] = ReadFileToString("../../Stray Cat/rooms/M.txt");
    rooms['N'] = ReadFileToString("../../Stray Cat/rooms/N.txt");
    rooms['W'] = ReadFileToString("../../Stray Cat/rooms/W.txt");
    
    Image gameOver("../../Stray Cat/resources/game_over.png");
    Image youWin("../../Stray Cat/resources/you_win.png");
    
    char currentRoomType = 'A';
    int currentRoomNumber = 0;
    Image currentBackground("../../Stray Cat/resources/grass-background.png");
    MakeBackground(rooms[currentRoomType], currentBackground);
    
    Image img("../../Stray Cat/resources/grass-background.png");
    MakeBackground(rooms[currentRoomType], img);
    AddUnstableElements(rooms[currentRoomType], img);
    
    for (int i = 0; i < img.Width(); i++) {
        for (int j = 0; j < img.Height(); j++) {
            screenBuffer.PutPixel(i, j, img.GetPixel(i, j));
        }
    }
    
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        for (int j = 0; j < tileSize; j++) {
            screenBuffer.PutPixel(i, WINDOW_HEIGHT - j - 1, brown);
        }
    }
    
    Image score_carrot("../../Stray Cat/resources/score_carrot.png");
    int screen_x = 0;
    int screen_y = WINDOW_HEIGHT - tileSize;
    for (int i = 0; i < tileSize; i++) {
        for (int j = 0; j < tileSize; j++) {
            screenBuffer.PutPixel(screen_x + i, screen_y + j,
                                  Blend(screenBuffer.GetPixel(screen_x + i, screen_y + j), score_carrot.GetPixel(i, tileSize - j - 1)));
        }
    }
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
    
        //game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        
        try {
            processPlayerMovement(player, rooms[currentRoomType], currentBackground, screenBuffer);
        } catch (char c) {
            int h;
            int w;
            int screen_x;
            int screen_y;
            switch (c) {
                case 'F':
                    h = gameOver.Height();
                    w = gameOver.Width();
                    screen_x = (WINDOW_WIDTH - w) / 2;
                    screen_y = (WINDOW_HEIGHT - h) / 2;
                    for (int i = 0; i < w; i++) {
                        for (int j = 0; j < h; j++) {
                            screenBuffer.PutPixel(screen_x + i, screen_y + j, gameOver.GetPixel(i, h - j - 1));
                        }
                    }
                    break;
                case 'W':
                    h = youWin.Height();
                    w = youWin.Width();
                    screen_x = (WINDOW_WIDTH - w) / 2;
                    screen_y = (WINDOW_HEIGHT - h) / 2;
                    for (int i = 0; i < w; i++) {
                        for (int j = 0; j < h; j++) {
                            screenBuffer.PutPixel(screen_x + i, screen_y + j, youWin.GetPixel(i, h - j - 1));
                        }
                    }
                    break;
                case 'U':
                    currentRoomNumber -= 6;
                    currentRoomType = plan[currentRoomNumber];
                    MakeBackground(rooms[currentRoomType], currentBackground);
                    MakeBackground(rooms[currentRoomType], img);
                    AddUnstableElements(rooms[currentRoomType], img);
                    for (int i = 0; i < img.Width(); i++) {
                        for (int j = 0; j < img.Height(); j++) {
                            screenBuffer.PutPixel(i, j, img.GetPixel(i, j));
                        }
                    }
                    player.SetCoords(WINDOW_WIDTH / 2 - tileSize / 2, tileSize);
                    break;
                case 'D':
                    currentRoomNumber += 6;
                    currentRoomType = plan[currentRoomNumber];
                    MakeBackground(rooms[currentRoomType], currentBackground);
                    MakeBackground(rooms[currentRoomType], img);
                    AddUnstableElements(rooms[currentRoomType], img);
                    for (int i = 0; i < img.Width(); i++) {
                        for (int j = 0; j < img.Height(); j++) {
                            screenBuffer.PutPixel(i, j, img.GetPixel(i, j));
                        }
                    }
                    player.SetCoords(WINDOW_WIDTH / 2 - tileSize / 2, WINDOW_HEIGHT - tileSize * 2 - playerHeight - 1);
                    break;
                case 'R':
                    currentRoomNumber += 1;
                    currentRoomType = plan[currentRoomNumber];
                    MakeBackground(rooms[currentRoomType], currentBackground);
                    MakeBackground(rooms[currentRoomType], img);
                    AddUnstableElements(rooms[currentRoomType], img);
                    for (int i = 0; i < img.Width(); i++) {
                        for (int j = 0; j < img.Height(); j++) {
                            screenBuffer.PutPixel(i, j, img.GetPixel(i, j));
                        }
                    }
                    player.SetCoords(tileSize, WINDOW_HEIGHT / 2 - playerHeight / 2 - tileSize / 2);
                    break;
                case 'L':
                    currentRoomNumber -= 1;
                    currentRoomType = plan[currentRoomNumber];
                    MakeBackground(rooms[currentRoomType], currentBackground);
                    MakeBackground(rooms[currentRoomType], img);
                    AddUnstableElements(rooms[currentRoomType], img);
                    for (int i = 0; i < img.Width(); i++) {
                        for (int j = 0; j < img.Height(); j++) {
                            screenBuffer.PutPixel(i, j, img.GetPixel(i, j));
                        }
                    }
                    player.SetCoords(WINDOW_WIDTH - tileSize * 2 - 1, WINDOW_HEIGHT / 2 - playerHeight / 2 - tileSize / 2);
                    break;
                default:
                    break;
            }
        }
        player.Draw(screenBuffer, currentBackground);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
