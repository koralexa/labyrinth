#include "Player.h"

#include <iostream>

Image player_img("../../Stray Cat/resources/player.png");
Image static_background("../../Stray Cat/resources/grass-background.png");

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

PlayerState CheckTilesUp(int move_dist, Point old_coords, std::string &room) {
    Point new_coords {.x = old_coords.x, .y = old_coords.y + move_dist};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - (new_coords.y + playerHeight) / tileSize; // new left up tile of player
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerState::STAY;
            break;
        case ' ':
            return PlayerState::DIE;
            break;
        default:
            break;
    }
    
    if (new_coords.x % tileSize != 0) {
        switch (room[tile_y * 16 + tile_x + 1]) {
            case '#':
                return PlayerState::STAY;
                break;
            case ' ':
                return PlayerState::DIE;
                break;
            default:
                break;
        }
    }
    
    return PlayerState::MOVE;
}

PlayerState CheckTilesDown(int move_dist, Point old_coords, std::string &room) {
    Point new_coords {.x = old_coords.x, .y = old_coords.y - move_dist};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - new_coords.y / tileSize; // new left down tile of player
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerState::STAY;
            break;
        case ' ':
            return PlayerState::DIE;
            break;
        default:
            break;
    }
    
    if (new_coords.x % tileSize != 0) {
        switch (room[tile_y * 16 + tile_x + 1]) {
            case '#':
                return PlayerState::STAY;
                break;
            case ' ':
                return PlayerState::DIE;
                break;
            default:
                break;
        }
    }
   
    return PlayerState::MOVE;
}

PlayerState CheckTilesLeft(int move_dist, Point old_coords, std::string &room) {
    Point new_coords {.x = old_coords.x - move_dist, .y = old_coords.y};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - new_coords.y / tileSize; // new left down tile of player
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerState::STAY;
            break;
        case ' ':
            return PlayerState::DIE;
            break;
        default:
            break;
    }
    
    switch (room[(tile_y - 1) * 16 + tile_x]) {
        case '#':
            return PlayerState::STAY;
            break;
        case ' ':
            return PlayerState::DIE;
            break;
        default:
            break;
    }
    
    if (new_coords.y % tileSize > tileSize * 2 - playerHeight) {
        switch (room[(tile_y - 2) * 16 + tile_x]) {
            case '#':
                return PlayerState::STAY;
                break;
            case ' ':
                return PlayerState::DIE;
                break;
            default:
                break;
        }
    }

    return PlayerState::MOVE;
}

PlayerState CheckTilesRight(int move_dist, Point old_coords, std::string &room) {
    Point new_coords {.x = old_coords.x + move_dist, .y = old_coords.y};
    int tile_x = new_coords.x / tileSize + 1;
    int tile_y = 15 - new_coords.y / tileSize; // new right down tile of player

    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerState::STAY;
            break;
        case ' ':
            return PlayerState::DIE;
            break;
        default:
            break;
    }
    
    switch (room[(tile_y - 1) * 16 + tile_x]) {
        case '#':
            return PlayerState::STAY;
            break;
        case ' ':
            return PlayerState::DIE;
            break;
        default:
            break;
    }
    
    if (new_coords.y % tileSize > tileSize * 2 - playerHeight) {
        switch (room[(tile_y - 2) * 16 + tile_x]) {
            case '#':
                return PlayerState::STAY;
                break;
            case ' ':
                return PlayerState::DIE;
                break;
            default:
                break;
        }
    }
    
    return PlayerState::MOVE;
}

void Player::ProcessInput(MovementDir dir, std::string &room)
{
  int move_dist = move_speed * 1;
    
  switch(dir)
  {
    case MovementDir::UP:
      if (CheckTilesUp(move_dist, coords, room) == PlayerState::MOVE) {
          old_coords.y = coords.y;
          coords.y += move_dist;
      }
      break;
    case MovementDir::DOWN:
      if (CheckTilesDown(move_dist, coords, room) == PlayerState::MOVE) {
          old_coords.y = coords.y;
          coords.y -= move_dist;
      }
      break;
    case MovementDir::LEFT:
      if (CheckTilesLeft(move_dist, coords, room) == PlayerState::MOVE) {
          old_coords.x = coords.x;
          coords.x -= move_dist;
      }
      break;
    case MovementDir::RIGHT:
      if (CheckTilesRight(move_dist, coords, room) == PlayerState::MOVE) {
          old_coords.x = coords.x;
          coords.x += move_dist;
      }
      break;
    default:
      break;
  }
}

void Player::Draw(Image &screen)
{
  if(Moved())
  {
    for(int y = old_coords.y; y < old_coords.y + playerHeight; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + playerWidth; ++x)
      {
        screen.PutPixel(x, y, static_background.GetPixel(x, static_background.Height() - y - 1));
      }
    }
    old_coords = coords;
  }

  for(int y = coords.y; y < coords.y + playerHeight; ++y)
  {
    for(int x = coords.x; x < coords.x + playerWidth; ++x)
    {
      screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), player_img.GetPixel(x - coords.x, playerHeight - (y - coords.y) - 1)));
    }
  }
}
