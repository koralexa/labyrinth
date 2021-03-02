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
#include <vector>
#include <fstream>

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
    
    for (int j = 0; j < grass.Height(); j++) {
        for (int i = 0; i < grass.Width(); i++) {
            screen.PutPixel(i, j, grass.GetPixel(i, grass.Height() - j - 1));
        }
    }
    
    for (int i = 0; i < room.size(); i++) {
        switch (room[i]) {
            case '#':
                for (int y = 0; y < tileSize; y++) {
                    for (int x = 0; x < tileSize; x++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), bush.GetPixel(x, tileSize - y - 1)));
                    }
                }
                break;
            case ' ':
                for (int y = 0; y < tileSize; y++) {
                    for (int x = 0; x < tileSize; x++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), pit.GetPixel(x, tileSize - y - 1)));
                    }
                }
                break;
            case 'Q':
                if (room[i + 1] == 'Q') {
                    for (int y = 0; y < tileSize; y++) {
                        for (int x = 0; x < tileSize * 2; x++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), finish_portal.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                }
                break;
            case 'X':
                if ((room[i + 1] == 'X') && (i / 16 < 8) && (i % 16 != 15)) {
                    for (int y = 0; y < tileSize; y++) {
                        for (int x = 0; x < tileSize * 2; x++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_up.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 1] == 'X') && (i / 16 >= 8) && (i % 16 != 15)) {
                    for (int y = 0; y < tileSize; y++) {
                        for (int x = 0; x < tileSize * 2; x++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (15 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_down.GetPixel(x, tileSize - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 16] == 'X') && (i % 16 < 8)) {
                    for (int y = 0; y < tileSize * 2; y++) {
                        for (int x = 0; x < tileSize; x++) {
                            screen_x = i % 16 * tileSize + x;
                            screen_y = (14 - i / 16) * tileSize + y;
                            screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y), portal_left.GetPixel(x, tileSize * 2 - y - 1)));
                        }
                    }
                    break;
                }
                if ((room[i + 16] == 'X') && (i % 16 >= 8)) {
                    for (int y = 0; y < tileSize * 2; y++) {
                        for (int x = 0; x < tileSize; x++) {
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

void AddUnstableElements(std::string &room, Image &screen, Image &background, int image_no) {
    Image carrots("../../Stray Cat/resources/carrots.png");
    Image hearts("../../Stray Cat/resources/hearts.png");
    
    int screen_x;
    int screen_y;
    
    int sprite_x = image_no / 10 * tileSize;
    
    for (int i = 0; i < room.size(); i++) {
        switch (room[i]) {
            case 'G':
                for (int y = 0; y < tileSize; y++) {
                    for (int x = 0; x < tileSize; x++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, background.GetPixel(screen_x, screen_y));
                    }
                }
                for (int y = 0; y < tileSize; y++) {
                    for (int x = 0; x < tileSize; x++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y),
                                                                  carrots.GetPixel(sprite_x + x, tileSize - y - 1)));
                    }
                }
                break;
              case 'H':
                for (int y = 0; y < tileSize; y++) {
                    for (int x = 0; x < tileSize; x++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, background.GetPixel(screen_x, screen_y));
                    }
                }
                for (int y = 0; y < tileSize; y++) {
                    for (int x = 0; x < tileSize; x++) {
                        screen_x = i % 16 * tileSize + x;
                        screen_y = (15 - i / 16) * tileSize + y;
                        screen.PutPixel(screen_x, screen_y, Blend(screen.GetPixel(screen_x, screen_y),
                                                                  hearts.GetPixel(sprite_x + x, tileSize - y - 1)));
                    }
                }
                break;
            default:
                break;
        }
    }
}

Pixel TransformPixel(Pixel p1, Pixel p2, int counter) {
    Pixel new_p;
    if (p1.r > p2.r) {
        new_p.r = p1.r - (p1.r - p2.r) / counter;
    } else {
        new_p.r = p1.r + (p2.r - p1.r) / counter;
    }
    if (p1.g > p2.g) {
        new_p.g = p1.g - (p1.g - p2.g) / counter;
    } else {
        new_p.g = p1.g + (p2.g - p1.g) / counter;
    }
    if (p1.b > p2.b) {
        new_p.b = p1.b - (p1.b - p2.b) / counter;
    } else {
        new_p.b = p1.b + (p2.b - p1.b) / counter;
    }
    return new_p;
}

int main(int argc, char** argv)
{
    if(!glfwInit())
        return -1;
    
        //    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bobby Carrot Labyrinth", nullptr, nullptr);
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
    
    std::vector<std::string> rooms;
    
    for (int i = 0; i < 36; i++) {
        switch (plan[i]) {
            case 'A':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/A.txt"));
                break;
            case 'B':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/B.txt"));
                break;
            case 'C':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/C.txt"));
                break;
            case 'D':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/D.txt"));
                break;
            case 'E':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/E.txt"));
                break;
            case 'F':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/F.txt"));
                break;
            case 'G':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/G.txt"));
                break;
            case 'H':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/H.txt"));
                break;
            case 'I':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/I.txt"));
                break;
            case 'J':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/J.txt"));
                break;
            case 'K':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/K.txt"));
                break;
            case 'L':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/L.txt"));
                break;
            case 'M':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/M.txt"));
                break;
            case 'N':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/N.txt"));
                break;
            case 'W':
                rooms.push_back(ReadFileToString("../../Stray Cat/rooms/W.txt"));
                break;
            default:
                rooms.push_back("");
                break;
        }
    }
    
    Image gameOver("../../Stray Cat/resources/game_over.png");
    Image youWin("../../Stray Cat/resources/you_win.png");
    
    int currentRoomNumber = 0;
    Image currentBackground("../../Stray Cat/resources/grass-background.png");
    MakeBackground(rooms[currentRoomNumber], currentBackground);
    
    Image img("../../Stray Cat/resources/grass-background.png");
    MakeBackground(rooms[currentRoomNumber], img);
    AddUnstableElements(rooms[currentRoomNumber], img, currentBackground, 0);
    
    for (int j = 0; j < img.Height(); j++) {
        for (int i = 0; i < img.Width(); i++) {
            screenBuffer.PutPixel(i, j, img.GetPixel(i, j));
        }
    }
    
    player.DrawCarrots(screenBuffer);
    player.DrawLives(screenBuffer);
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
    
    GLfloat timeToStart = 0;
    
    int frame_type = 0;
    int finish_count = 0;
    int win_count = 0;
    int switch_level_count = 0;
    
        //game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        
        frame_type = (frame_type + 1) % 60;
        if ((player.GetPlayerAction() != PlayerAction::DIE) &&
            (player.GetPlayerAction() != PlayerAction::WIN) &&
            (switch_level_count == 0)) {
            AddUnstableElements(rooms[currentRoomNumber], screenBuffer, currentBackground, frame_type);
        }
        
        if (!player.GetActive() && (currentFrame > timeToStart) && (timeToStart != 0)) {
            player.SetActive(true);
            timeToStart = 0;
        }
        
        if (finish_count > 0) {
            for (int j = 0; j < screenBuffer.Height() - tileSize; j++) {
                for (int i = 0; i < screenBuffer.Width(); i++) {
                    screenBuffer.PutPixel(i, j, TransformPixel(screenBuffer.GetPixel(i, j),
                                                               gameOver.GetPixel(i, gameOver.Height() - j - 1), finish_count));
                }
            }
            finish_count -= 1;
        }
        
        if (win_count > 0) {
            for (int j = 0; j < screenBuffer.Height() - tileSize; j++) {
                for (int i = 0; i < screenBuffer.Width(); i++) {
                    screenBuffer.PutPixel(i, j, TransformPixel(screenBuffer.GetPixel(i, j),
                                                               youWin.GetPixel(i, youWin.Height() - j - 1), win_count));
                }
            }
            win_count -= 1;
        }
        
        if (switch_level_count > 0) {
            for (int j = 0; j < screenBuffer.Height(); j++) {
                for (int i = 0; i < screenBuffer.Width(); i++) {
                    screenBuffer.PutPixel(i, j, TransformPixel(screenBuffer.GetPixel(i, j), img.GetPixel(i, j), switch_level_count));
                }
            }
        }
        
        try {
            if (switch_level_count == 0) {
                processPlayerMovement(player, rooms[currentRoomNumber], currentBackground, screenBuffer);
                player.Draw(screenBuffer, currentBackground);
            } else {
                switch_level_count -= 1;
            }
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
                    timeToStart = 0;
                    finish_count = 32;
                    break;
                case 'W':
                    h = youWin.Height();
                    w = youWin.Width();
                    screen_x = (WINDOW_WIDTH - w) / 2;
                    screen_y = (WINDOW_HEIGHT - h) / 2;
                    win_count = 32;
                    break;
                case 'U':
                    currentRoomNumber -= 6;
                    MakeBackground(rooms[currentRoomNumber], currentBackground);
                    MakeBackground(rooms[currentRoomNumber], img);
                    AddUnstableElements(rooms[currentRoomNumber], img, currentBackground, 4);
                    player.DrawCarrots(img);
                    player.DrawLives(img);
                    switch_level_count = 16;
                    player.SetCoords(WINDOW_WIDTH / 2 - tileSize / 2, 0);
                    player.SetGettingOutCount(36);
                    player.SetCurrentImage(16);
                    break;
                case 'D':
                    currentRoomNumber += 6;
                    MakeBackground(rooms[currentRoomNumber], currentBackground);
                    MakeBackground(rooms[currentRoomNumber], img);
                    AddUnstableElements(rooms[currentRoomNumber], img, currentBackground, 4);
                    player.DrawCarrots(img);
                    player.DrawLives(img);
                    switch_level_count = 16;
                    player.SetCoords(WINDOW_WIDTH / 2 - tileSize / 2, WINDOW_HEIGHT - tileSize - playerHeight - 1);
                    player.SetGettingOutCount(36);
                    player.SetCurrentImage(24);
                    break;
                case 'R':
                    currentRoomNumber += 1;
                    MakeBackground(rooms[currentRoomNumber], currentBackground);
                    MakeBackground(rooms[currentRoomNumber], img);
                    AddUnstableElements(rooms[currentRoomNumber], img, currentBackground, 4);
                    player.DrawCarrots(img);
                    player.DrawLives(img);
                    switch_level_count = 16;
                    player.SetCoords(0, WINDOW_HEIGHT / 2 - playerHeight / 2 - tileSize / 2);
                    player.SetGettingOutCount(36);
                    player.SetCurrentImage(8);
                    break;
                case 'L':
                    currentRoomNumber -= 1;
                    MakeBackground(rooms[currentRoomNumber], currentBackground);
                    MakeBackground(rooms[currentRoomNumber], img);
                    AddUnstableElements(rooms[currentRoomNumber], img, currentBackground, 4);
                    player.DrawCarrots(img);
                    player.DrawLives(img);
                    switch_level_count = 16;
                    player.SetCoords(WINDOW_WIDTH - tileSize - 1, WINDOW_HEIGHT / 2 - playerHeight / 2 - tileSize / 2);
                    player.SetGettingOutCount(36);
                    player.SetCurrentImage(0);
                    break;
                case 'S':
                    timeToStart = currentFrame + 0.7;
                    break;
                default:
                    break;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
