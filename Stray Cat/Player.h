#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <iostream>
#include <string>

constexpr int playerWidth = 64;
constexpr int playerHeight = 98;

struct Point
{
  int x;
  int y;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum class PlayerAction
{
    STAY,
    MOVE,
    DIE,
    FALL,
    CARROT1,
    CARROT2,
    CARROT3,
    PORTAL_UP,
    PORTAL_DOWN,
    PORTAL_RIGHT,
    PORTAL_LEFT,
    WIN
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                  coords(pos), old_coords(coords) {};


  bool Moved() const;
  void ProcessInput(MovementDir dir, std::string &room, Image &currentBackground, Image &screen);
  void Draw(Image &screen, Image &currentBackground);
  
  bool GetActive() { return active; }
  void SetActive(bool a) { active = a; }
  void SetCoords(int x, int y) { coords.x = x; coords.y = y; }
  void SetGettingOutCount(int g) { getting_out_count = g; }
  void SetCurrentImage(int i) { current_image = i; }
  enum PlayerAction GetPlayerAction() { return player_action; }
  void DrawCarrots(Image &screen);
  void DrawLives(Image &screen);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 4;
  int carrots = 0;
  int lives = 3;
  bool active = true;
  int current_image = 8;
  int switch_image = 3;
  enum PlayerAction player_action = PlayerAction::MOVE;
  int falling_count = 0;
  int getting_out_count = 0;
  int dying_count = 0;
};

#endif //MAIN_PLAYER_H
