#include "Player.h"

#include <iostream>

Image player_img("../../Stray Cat/resources/player.png");

Image score_carrot("../../Stray Cat/resources/score_carrot.png");

Image zero("../../Stray Cat/resources/0.png");
Image one("../../Stray Cat/resources/1.png");
Image two("../../Stray Cat/resources/2.png");
Image three("../../Stray Cat/resources/3.png");
Image four("../../Stray Cat/resources/4.png");
Image five("../../Stray Cat/resources/5.png");
Image six("../../Stray Cat/resources/6.png");
Image seven("../../Stray Cat/resources/7.png");
Image eight("../../Stray Cat/resources/8.png");
Image nine("../../Stray Cat/resources/9.png");
Image slash("../../Stray Cat/resources/slash.png");

Image full_heart("../../Stray Cat/resources/full_heart.png");
Image heart("../../Stray Cat/resources/heart.png");

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
            carrots++;
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
    
  if (!active) {
    return;
  }
    
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
          DrawCarrots(screen);
          break;
      case PlayerAction::CARROT2:
          carrots += 2;
          DrawCarrots(screen);
          break;
      case PlayerAction::CARROT3:
          carrots += 3;
          DrawCarrots(screen);
          break;
      case PlayerAction::DIE:
          lives -= 1;
          DrawLives(screen);
          active = false;
          if (lives > 0) {
              throw ('S');
          } else {
              throw('F');
          }
          break;
      default:
          break;
  }
}

void DrawDigit(Image &screen, Image &digit, int screen_x, int screen_y) {
    for (int i = 0; i < tileSize; i++) {
        for (int j = 0; j < tileSize; j++) {
            screen.PutPixel(screen_x + i, screen_y + j,
                            Blend(screen.GetPixel(screen_x + i, screen_y + j), digit.GetPixel(i, tileSize - j - 1)));
        }
    }
}

void SwitchDigit(Image &screen, int digit, int screen_x, int screen_y) {
    switch (digit) {
        case 0:
            DrawDigit(screen, zero, screen_x, screen_y);
            break;
        case 1:
            DrawDigit(screen, one, screen_x, screen_y);
            break;
        case 2:
            DrawDigit(screen, two, screen_x, screen_y);
            break;
        case 3:
            DrawDigit(screen, three, screen_x, screen_y);
            break;
        case 4:
            DrawDigit(screen, four, screen_x, screen_y);
            break;
        case 5:
            DrawDigit(screen, five, screen_x, screen_y);
            break;
        case 6:
            DrawDigit(screen, six, screen_x, screen_y);
            break;
        case 7:
            DrawDigit(screen, seven, screen_x, screen_y);
            break;
        case 8:
            DrawDigit(screen, eight, screen_x, screen_y);
            break;
        case 9:
            DrawDigit(screen, nine, screen_x, screen_y);
            break;
        default:
            break;
    }
}

void Player::DrawCarrots(Image &screen) {
    for (int i = 0; i < screen.Width() / 2; i++) {
        for (int j = 0; j < tileSize; j++) {
            screen.PutPixel(i, screen.Height() - j - 1, brown);
        }
    }
    
    int screen_x = 0;
    int screen_y = screen.Height() - tileSize;
    for (int i = 0; i < tileSize; i++) {
        for (int j = 0; j < tileSize; j++) {
            screen.PutPixel(screen_x + i, screen_y + j,
                                  Blend(screen.GetPixel(screen_x + i, screen_y + j), score_carrot.GetPixel(i, tileSize - j - 1)));
        }
    }
    
    screen_x = tileSize;
    
    if (carrots / 100 != 0) {
        DrawDigit(screen, one, screen_x, screen_y);
        screen_x += tileSize;
    }
    
    if ((carrots / 100 != 0) || (carrots / 10 != 0)) {
        SwitchDigit(screen, carrots % 100 / 10, screen_x, screen_y);
        screen_x += tileSize;
    }
    
    SwitchDigit(screen, carrots % 10, screen_x, screen_y);
    screen_x += tileSize;
    
    for (int i = 0; i < tileSize; i++) {
        for (int j = 0; j < tileSize; j++) {
            screen.PutPixel(screen_x + i, screen_y + j,
                                  Blend(screen.GetPixel(screen_x + i, screen_y + j), slash.GetPixel(i, tileSize - j - 1)));
        }
    }
    screen_x += tileSize;
    
    DrawDigit(screen, one, screen_x, screen_y);
    screen_x += tileSize;
    DrawDigit(screen, three, screen_x, screen_y);
    screen_x += tileSize;
    DrawDigit(screen, seven, screen_x, screen_y);
}

void Player::DrawLives(Image &screen) {
    for (int i = screen.Width() / 2; i < screen.Width(); i++) {
        for (int j = 0; j < tileSize; j++) {
            screen.PutPixel(i, screen.Height() - j - 1, brown);
        }
    }
    
    int screen_x = screen.Width() - tileSize * 3;
    int screen_y = screen.Height() - tileSize;
    
    for (int k = 1; k <= 3 - lives; k++) {
        for (int i = 0; i < tileSize; i++) {
            for (int j = 0; j < tileSize; j++) {
                screen.PutPixel(screen_x + i, screen_y + j,
                                      Blend(screen.GetPixel(screen_x + i, screen_y + j), heart.GetPixel(i, tileSize - j - 1)));
            }
        }
        screen_x += tileSize;
    }
    
    for (int k = 1; k <= lives; k++) {
        for (int i = 0; i < tileSize; i++) {
            for (int j = 0; j < tileSize; j++) {
                screen.PutPixel(screen_x + i, screen_y + j,
                                      Blend(screen.GetPixel(screen_x + i, screen_y + j), full_heart.GetPixel(i, tileSize - j - 1)));
            }
        }
        screen_x += tileSize;
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
