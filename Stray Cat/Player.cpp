#include "Player.h"

#include <iostream>

Image player_img("../../Stray Cat/resources/player.png");

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

PlayerAction CheckTilesUp(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x, .y = old_coords.y + move_dist};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - (new_coords.y + playerHeight) / tileSize; // new left up tile of player
    
    int carrots = 0;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
            room[tile_y * 16 + tile_x] = '.';
            for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                }
            }
            carrots++;
            break;
        case 'X':
            return PlayerAction::PORTAL;
            break;
        default:
            break;
    }
    
    if (new_coords.x % tileSize != 0) {
        switch (room[tile_y * 16 + tile_x + 1]) {
            case '#':
                return PlayerAction::STAY;
                break;
            case ' ':
                return PlayerAction::DIE;
                break;
            case 'G':
                room[tile_y * 16 + tile_x + 1] = '.';
                for (int x = (tile_x + 1) * tileSize; x < (tile_x + 1) * tileSize + tileSize; x++) {
                    for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                    }
                }
                carrots++;
                break;
            case 'X':
                return PlayerAction::PORTAL;
                break;
            default:
                break;
        }
    }
    
    switch (carrots) {
        case 1:
            return PlayerAction::CARROT1;
        case 2:
            return PlayerAction::CARROT2;
        case 3:
            return PlayerAction::CARROT3;
        default:
            break;
    }
    
    return PlayerAction::MOVE;
}

PlayerAction CheckTilesDown(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x, .y = old_coords.y - move_dist};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - new_coords.y / tileSize; // new left down tile of player
    
    int carrots = 0;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
            room[tile_y * 16 + tile_x] = '.';
            for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                }
            }
            carrots++;;
            break;
        case 'X':
            return PlayerAction::PORTAL;
            break;
        case 'Q':
            return PlayerAction::WIN;
            break;
        default:
            break;
    }
    
    if (new_coords.x % tileSize != 0) {
        switch (room[tile_y * 16 + tile_x + 1]) {
            case '#':
                return PlayerAction::STAY;
                break;
            case ' ':
                return PlayerAction::DIE;
                break;
            case 'G':
                room[tile_y * 16 + tile_x + 1] = '.';
                for (int x = (tile_x + 1) * tileSize; x < (tile_x + 1) * tileSize + tileSize; x++) {
                    for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                    }
                }
                carrots++;;
                break;
            case 'X':
                return PlayerAction::PORTAL;
                break;
            case 'Q':
                return PlayerAction::WIN;
                break;
            default:
                break;
        }
    }
    
    switch (carrots) {
        case 1:
            return PlayerAction::CARROT1;
        case 2:
            return PlayerAction::CARROT2;
        case 3:
            return PlayerAction::CARROT3;
        default:
            break;
    }
   
    return PlayerAction::MOVE;
}

PlayerAction CheckTilesLeft(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x - move_dist, .y = old_coords.y};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - new_coords.y / tileSize; // new left down tile of player
    
    int carrots = 0;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
            room[tile_y * 16 + tile_x] = '.';
            for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                }
            }
            carrots++;;
            break;
        case 'X':
            return PlayerAction::PORTAL;
            break;
        default:
            break;
    }
    
    switch (room[(tile_y - 1) * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
            room[(tile_y - 1) * 16 + tile_x] = '.';
            for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                for (int y = (tile_y - 1) * tileSize; y < (tile_y - 1) * tileSize + tileSize; y++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                }
            }
            carrots++;;
            break;
        case 'X':
            return PlayerAction::PORTAL;
            break;
        default:
            break;
    }
    
    if (new_coords.y % tileSize > tileSize * 2 - playerHeight) {
        switch (room[(tile_y - 2) * 16 + tile_x]) {
            case '#':
                return PlayerAction::STAY;
                break;
            case ' ':
                return PlayerAction::DIE;
                break;
            case 'G':
                room[(tile_y - 2) * 16 + tile_x] = '.';
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    for (int y = (tile_y - 2) * tileSize; y < (tile_y - 2) * tileSize + tileSize; y++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                    }
                }
                carrots++;;
                break;
            case 'X':
                return PlayerAction::PORTAL;
                break;
            default:
                break;
        }
    }
    
    switch (carrots) {
        case 1:
            return PlayerAction::CARROT1;
        case 2:
            return PlayerAction::CARROT2;
        case 3:
            return PlayerAction::CARROT3;
        default:
            break;
    }

    return PlayerAction::MOVE;
}

PlayerAction CheckTilesRight(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x + move_dist, .y = old_coords.y};
    int tile_x = new_coords.x / tileSize + 1;
    int tile_y = 15 - new_coords.y / tileSize; // new right down tile of player
    
    int carrots = 0;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
            room[tile_y * 16 + tile_x] = '.';
            for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                }
            }
            carrots++;;
            break;
        case 'X':
            return PlayerAction::PORTAL;
            break;
        default:
            break;
    }
    
    switch (room[(tile_y - 1) * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
            room[(tile_y - 1) * 16 + tile_x] = '.';
            for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                for (int y = (tile_y - 1) * tileSize; y < (tile_y - 1) * tileSize + tileSize; y++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                }
            }
            carrots++;;
            break;
        case 'X':
            return PlayerAction::PORTAL;
            break;
        default:
            break;
    }
    
    if (new_coords.y % tileSize > tileSize * 2 - playerHeight) {
        switch (room[(tile_y - 2) * 16 + tile_x]) {
            case '#':
                return PlayerAction::STAY;
                break;
            case ' ':
                return PlayerAction::DIE;
                break;
            case 'G':
                room[(tile_y - 2) * 16 + tile_x] = '.';
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    for (int y = (tile_y - 2) * tileSize; y < (tile_y - 2) * tileSize + tileSize; y++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1, currentBackground.GetPixel(x, currentBackground.Height() - y - 1));
                    }
                }
                carrots++;;
                break;
            case 'X':
                return PlayerAction::PORTAL;
                break;
            default:
                break;
        }
    }
    
    switch (carrots) {
        case 1:
            return PlayerAction::CARROT1;
        case 2:
            return PlayerAction::CARROT2;
        case 3:
            return PlayerAction::CARROT3;
        default:
            break;
    }
    
    return PlayerAction::MOVE;
}

void Player::ProcessInput(MovementDir dir, std::string &room, Image &currentBackground, Image &screen)
{
  int move_dist = move_speed * 1;
    
  PlayerAction action;
    
  switch(dir)
  {
    case MovementDir::UP:
      action = CheckTilesUp(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL)) {
          old_coords.y = coords.y;
          coords.y += move_dist;
          if (action == PlayerAction::PORTAL) {
              throw('U');
          }
      }
      break;
    case MovementDir::DOWN:
      action = CheckTilesDown(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL) || (action == PlayerAction::WIN)) {
          old_coords.y = coords.y;
          coords.y -= move_dist;
          if (action == PlayerAction::PORTAL) {
              throw('D');
          }
          if (action == PlayerAction::WIN) {
              active = false;
              throw('W');
          }
      }
      break;
    case MovementDir::LEFT:
      action = CheckTilesLeft(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL)) {
          old_coords.x = coords.x;
          coords.x -= move_dist;
          if (action == PlayerAction::PORTAL) {
              throw('L');
          }
      }
      break;
    case MovementDir::RIGHT:
      action = CheckTilesRight(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL)) {
          old_coords.x = coords.x;
          coords.x += move_dist;
          if (action == PlayerAction::PORTAL) {
              throw('R');
          }
      }
      break;
    default:
      break;
  }
    
  switch (action) {
      case PlayerAction::CARROT1:
          carrots += 1;
          break;
      case PlayerAction::CARROT2:
          carrots += 2;
          break;
      case PlayerAction::CARROT3:
          carrots += 3;
          break;
      case PlayerAction::DIE:
          active = false;
          throw('F');
          break;
      default:
          break;
  }
}

void Player::Draw(Image &screen, Image &currentBackground)
{
  if(Moved() && active)
  {
    for(int y = old_coords.y; y < old_coords.y + playerHeight; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + playerWidth; ++x)
      {
        screen.PutPixel(x, y, currentBackground.GetPixel(x, y));
      }
    }
    old_coords = coords;
  }

  if (active) {
      for(int y = coords.y; y < coords.y + playerHeight; ++y)
      {
        for(int x = coords.x; x < coords.x + playerWidth; ++x)
        {
          screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), player_img.GetPixel(x - coords.x, playerHeight - (y - coords.y) - 1)));
        }
      }
  }
}
