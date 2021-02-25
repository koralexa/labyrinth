#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

constexpr int playerWidth = 64;
constexpr int playerHeight = 94;

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

enum class PlayerState
{
    STAY,
    MOVE,
    DIE
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  void ProcessInput(MovementDir dir, std::string &room);
  void Draw(Image &screen);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 4;

};

#endif //MAIN_PLAYER_H
