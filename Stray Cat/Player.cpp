#include "Player.h"

#include <iostream>
#include <vector>

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

Image Bobby_going("../../Stray Cat/resources/Bobby_going.png");
Image Bobby_falling("../../Stray Cat/resources/Bobby_falling.png");
Image Bobby_dying("../../Stray Cat/resources/Bobby_dying.png");

Image portals("../../Stray Cat/resources/portals.png");
Image finish_portals("../../Stray Cat/resources/finish_portals.png");

Image wolf_left("../../Stray Cat/resources/wolf_left.png");
Image wolf_right("../../Stray Cat/resources/wolf_right.png");
Image wolf_sleeping("../../Stray Cat/resources/wolf_sleeping.png");
Image wolf_firing_left("../../Stray Cat/resources/wolf_firing_left.png");
Image wolf_firing_right("../../Stray Cat/resources/wolf_firing_right.png");

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
    int hearts = 0;
    char tile;
    
    PlayerAction tmp_action = PlayerAction::MOVE;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
        case 'H':
            tile = room[tile_y * 16 + tile_x];
            room[tile_y * 16 + tile_x] = '.';
            for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                    currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                }
            }
            if (tile == 'H') {
                hearts++;
            } else {
                carrots++;
            }
            break;
        case 'X':
            tmp_action = PlayerAction::PORTAL_UP;
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
            case 'H':
                tile = room[tile_y * 16 + tile_x + 1];
                room[tile_y * 16 + tile_x + 1] = '.';
                for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                    for (int x = (tile_x + 1) * tileSize; x < (tile_x + 1) * tileSize + tileSize; x++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                        currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                    }
                }
                if (tile == 'H') {
                    hearts++;
                } else {
                    carrots++;
                }
                break;
            case 'X':
                tmp_action = PlayerAction::PORTAL_UP;
                break;
            default:
                break;
        }
    }
    
    if (tmp_action == PlayerAction::PORTAL_UP) {
        return tmp_action;
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
    
    if (hearts > 0) {
        return PlayerAction::HEART;
    }
    
    return PlayerAction::MOVE;
}

PlayerAction CheckTilesDown(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x, .y = old_coords.y - move_dist};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - new_coords.y / tileSize; // new left down tile of player
    
    int carrots = 0;
    int hearts = 0;
    char tile;
    
    PlayerAction tmp_action = PlayerAction::MOVE;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
        case 'H':
            tile = room[tile_y * 16 + tile_x];
            room[tile_y * 16 + tile_x] = '.';
            for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                    currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                }
            }
            if (tile == 'H') {
                hearts++;
            } else {
                carrots++;
            }
            break;
        case 'X':
            tmp_action = PlayerAction::PORTAL_DOWN;
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
            case 'H':
                tile = room[tile_y * 16 + tile_x + 1];
                room[tile_y * 16 + tile_x + 1] = '.';
                for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                    for (int x = (tile_x + 1) * tileSize; x < (tile_x + 1) * tileSize + tileSize; x++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                        currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                    }
                }
                if (tile == 'H') {
                    hearts++;
                } else {
                    carrots++;
                }
                break;
            case 'X':
                tmp_action = PlayerAction::PORTAL_DOWN;
                break;
            case 'Q':
                return PlayerAction::WIN;
                break;
            default:
                break;
        }
    }
    
    if (tmp_action == PlayerAction::PORTAL_DOWN) {
        return tmp_action;
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
    
    if (hearts > 0) {
        return PlayerAction::HEART;
    }
   
    return PlayerAction::MOVE;
}

PlayerAction CheckTilesLeft(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x - move_dist, .y = old_coords.y};
    int tile_x = new_coords.x / tileSize;
    int tile_y = 15 - new_coords.y / tileSize; // new left down tile of player
    
    int carrots = 0;
    int hearts = 0;
    char tile;
    
    PlayerAction tmp_action = PlayerAction::MOVE;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
        case 'H':
            tile = room[tile_y * 16 + tile_x];
            room[tile_y * 16 + tile_x] = '.';
            for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                    currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                }
            }
            if (tile == 'H') {
                hearts++;
            } else {
                carrots++;
            }
            break;
        case 'X':
            tmp_action = PlayerAction::PORTAL_LEFT;
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
        case 'H':
            tile = room[(tile_y - 1) * 16 + tile_x];
            room[(tile_y - 1) * 16 + tile_x] = '.';
            for (int y = (tile_y - 1) * tileSize; y < (tile_y - 1) * tileSize + tileSize; y++) {
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                    currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                }
            }
            if (tile == 'H') {
                hearts++;
            } else {
                carrots++;
            }
            break;
        case 'X':
            tmp_action = PlayerAction::PORTAL_LEFT;
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
            case 'H':
                tile = room[(tile_y - 2) * 16 + tile_x];
                room[(tile_y - 2) * 16 + tile_x] = '.';
                for (int y = (tile_y - 2) * tileSize; y < (tile_y - 2) * tileSize + tileSize; y++) {
                    for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                        currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                    }
                }
                if (tile == 'H') {
                    hearts++;
                } else {
                    carrots++;
                }
                break;
            case 'X':
                tmp_action = PlayerAction::PORTAL_LEFT;
                break;
            default:
                break;
        }
    }
    
    if (tmp_action == PlayerAction::PORTAL_LEFT) {
        return tmp_action;
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
    
    if (hearts > 0) {
        return PlayerAction::HEART;
    }

    return PlayerAction::MOVE;
}

PlayerAction CheckTilesRight(int move_dist, Point old_coords, std::string &room, Image &currentBackground, Image &screen) {
    Point new_coords {.x = old_coords.x + move_dist, .y = old_coords.y};
    int tile_x = new_coords.x / tileSize + 1;
    int tile_y = 15 - new_coords.y / tileSize; // new right down tile of player
    
    int carrots = 0;
    int hearts = 0;
    char tile;
    
    PlayerAction tmp_action = PlayerAction::MOVE;
    
    switch (room[tile_y * 16 + tile_x]) {
        case '#':
            return PlayerAction::STAY;
            break;
        case ' ':
            return PlayerAction::DIE;
            break;
        case 'G':
        case 'H':
            tile = room[tile_y * 16 + tile_x];
            room[tile_y * 16 + tile_x] = '.';
            for (int y = tile_y * tileSize; y < tile_y * tileSize + tileSize; y++) {
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                    currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                }
            }
            if (tile == 'H') {
                hearts++;
            } else {
                carrots++;
            }
            break;
        case 'X':
            tmp_action = PlayerAction::PORTAL_RIGHT;
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
        case 'H':
            tile = room[(tile_y - 1) * 16 + tile_x];
            room[(tile_y - 1) * 16 + tile_x] = '.';
            for (int y = (tile_y - 1) * tileSize; y < (tile_y - 1) * tileSize + tileSize; y++) {
                for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                    screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                    currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                }
            }
            if (tile == 'H') {
                hearts++;
            } else {
                carrots++;
            }
            break;
        case 'X':
            tmp_action = PlayerAction::PORTAL_RIGHT;
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
            case 'H':
                tile = room[(tile_y - 2) * 16 + tile_x];
                room[(tile_y - 2) * 16 + tile_x] = '.';
                for (int y = (tile_y - 2) * tileSize; y < (tile_y - 2) * tileSize + tileSize; y++) {
                    for (int x = tile_x * tileSize; x < tile_x * tileSize + tileSize; x++) {
                        screen.PutPixel(x, screen.Height() - tileSize - y - 1,
                                        currentBackground.GetPixel(x, currentBackground.Height() - tileSize - y - 1));
                    }
                }
                if (tile == 'H') {
                    hearts++;
                } else {
                    carrots++;
                }
                break;
            case 'X':
                tmp_action = PlayerAction::PORTAL_RIGHT;
                break;
            default:
                break;
        }
    }
    
    if (tmp_action == PlayerAction::PORTAL_RIGHT) {
        return tmp_action;
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
    
    if (hearts > 0) {
        return PlayerAction::HEART;
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
          (action == PlayerAction::PORTAL_UP) || (action == PlayerAction::HEART)) {
          old_coords.y = coords.y;
          coords.y += move_dist;
          if (switch_image == 3) {
              if (current_image / 8 == 2) {
                  current_image = 16 + (current_image + 1) % 8;
              } else {
                  current_image = 16;
              }
          }
          switch_image = (switch_image + 1) % 4;
          if (action == PlayerAction::PORTAL_UP) {
              active = false;
              player_action = PlayerAction::PORTAL_UP;
              last_portal = PlayerAction::PORTAL_UP;
              falling_count = 36;
          }
      }
      break;
    case MovementDir::DOWN:
      action = CheckTilesDown(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL_DOWN) || (action == PlayerAction::WIN)  ||
          (action == PlayerAction::HEART)) {
          old_coords.y = coords.y;
          coords.y -= move_dist;
          if (switch_image == 3) {
              if (current_image / 8 == 3) {
                  current_image = 24 + (current_image + 1) % 8;
              } else {
                  current_image = 24;
              }
          }
          switch_image = (switch_image + 1) % 4;
          if (action == PlayerAction::PORTAL_DOWN) {
              active = false;
              player_action = PlayerAction::PORTAL_DOWN;
              last_portal = PlayerAction::PORTAL_DOWN;
              falling_count = 36;
          }
          if (action == PlayerAction::WIN) {
              active = false;
              player_action = PlayerAction::WIN;
              falling_count = 36;
          }
      }
      break;
    case MovementDir::LEFT:
      action = CheckTilesLeft(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL_LEFT) || (action == PlayerAction::HEART)) {
          old_coords.x = coords.x;
          coords.x -= move_dist;
          if (switch_image == 3) {
              if (current_image / 8 == 0) {
                  current_image = (current_image + 1) % 8;
              } else {
                  current_image = 0;
              }
          }
          switch_image = (switch_image + 1) % 4;
          if (action == PlayerAction::PORTAL_LEFT) {
              active = false;
              player_action = PlayerAction::PORTAL_LEFT;
              last_portal = PlayerAction::PORTAL_LEFT;
              falling_count = 36;
          }
      }
      break;
    case MovementDir::RIGHT:
      action = CheckTilesRight(move_dist, coords, room, currentBackground, screen);
      if ((action == PlayerAction::MOVE) || (action == PlayerAction::CARROT1) ||
          (action == PlayerAction::CARROT2) || (action == PlayerAction::CARROT3) ||
          (action == PlayerAction::PORTAL_RIGHT)  || (action == PlayerAction::HEART)) {
          old_coords.x = coords.x;
          coords.x += move_dist;
          if (switch_image == 3) {
              if (current_image / 8 == 1) {
                  current_image = 8 + (current_image + 1) % 8;
              } else {
                  current_image = 8;
              }
          }
          switch_image = (switch_image + 1) % 4;
          if (action == PlayerAction::PORTAL_RIGHT) {
              active = false;
              player_action = PlayerAction::PORTAL_RIGHT;
              last_portal = PlayerAction::PORTAL_RIGHT;
              falling_count = 36;
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
      case PlayerAction::HEART:
          lives += 1;
          DrawLives(screen);
          break;
      case PlayerAction::DIE:
          lives -= 1;
          DrawLives(screen);
          active = false;
          if (lives > 0) {
              throw ('S');
          } else {
              dying_count = 32;
          }
          break;
      default:
          break;
  }
}

void DrawDigit(Image &screen, Image &digit, int screen_x, int screen_y) {
    for (int j = 0; j < tileSize; j++) {
        for (int i = 0; i < tileSize; i++) {
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
    for (int j = 0; j < tileSize; j++) {
        for (int i = 0; i < screen.Width() / 2; i++) {
            screen.PutPixel(i, screen.Height() - j - 1, brown);
        }
    }
    
    int screen_x = 0;
    int screen_y = screen.Height() - tileSize;
    for (int j = 0; j < tileSize; j++) {
        for (int i = 0; i < tileSize; i++) {
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
    
    for (int j = 0; j < tileSize; j++) {
        for (int i = 0; i < tileSize; i++) {
            screen.PutPixel(screen_x + i, screen_y + j,
                                  Blend(screen.GetPixel(screen_x + i, screen_y + j), slash.GetPixel(i, tileSize - j - 1)));
        }
    }
    screen_x += tileSize;
    
    DrawDigit(screen, one, screen_x, screen_y);
    screen_x += tileSize;
    DrawDigit(screen, seven, screen_x, screen_y);
    screen_x += tileSize;
    DrawDigit(screen, zero, screen_x, screen_y);
}

void Player::DrawLives(Image &screen) {
    for (int j = 0; j < tileSize; j++) {
        for (int i = screen.Width() / 2; i < screen.Width(); i++) {
            screen.PutPixel(i, screen.Height() - j - 1, brown);
        }
    }
    
    int screen_x = screen.Width() - tileSize * 8;
    int screen_y = screen.Height() - tileSize;
    
    for (int k = 1; k <= 8 - lives; k++) {
        for (int j = 0; j < tileSize; j++) {
            for (int i = 0; i < tileSize; i++) {
                screen.PutPixel(screen_x + i, screen_y + j,
                                      Blend(screen.GetPixel(screen_x + i, screen_y + j), heart.GetPixel(i, tileSize - j - 1)));
            }
        }
        screen_x += tileSize;
    }
    
    for (int k = 1; k <= lives; k++) {
        for (int j = 0; j < tileSize; j++) {
            for (int i = 0; i < tileSize; i++) {
                screen.PutPixel(screen_x + i, screen_y + j,
                                      Blend(screen.GetPixel(screen_x + i, screen_y + j), full_heart.GetPixel(i, tileSize - j - 1)));
            }
        }
        screen_x += tileSize;
    }
}

void Player::Draw(Image &screen, Image &currentBackground, std::vector<Wolf> &wolves)
{
  if(Moved() || (!active && (((falling_count > 0) && (player_action != PlayerAction::WOLF)) ||
                             (getting_out_count > 0) || (dying_count > 0))))
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
  DrawLives(screen);
  DrawCarrots(screen);

  if (active) {
      int image_x = (current_image % 8) * playerWidth;
      int image_y = (current_image / 8) * (playerHeight + 1);
      for(int y = coords.y; y < coords.y + playerHeight; ++y)
      {
        for(int x = coords.x; x < coords.x + playerWidth; ++x)
        {
          screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), Bobby_going.GetPixel(image_x + x - coords.x,
                                                                playerHeight + image_y - (y - coords.y) - 1)));
        }
      }
  } else if (falling_count > 0) {
      int image_x;
      int image_y;
      if (falling_count > 20) {
          image_x = ((36 - falling_count + 1) / 2) * (tileSize * 2 + 6);
      } else if (falling_count > 6) {
          image_x = (7 - ((22 - falling_count + 1) / 2)) * (tileSize * 2 + 6);
      } else {
          image_x = ((6 - falling_count + 1) / 2 + 1) * (tileSize * 2 + 6);
      }
      image_y = 0;
      switch (player_action) {
          case PlayerAction::PORTAL_UP:
              for(int y = tileSize * 15; y < tileSize * 16; ++y)
              {
                for(int x = tileSize * 7; x < tileSize * 9; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x - tileSize * 7,
                                                                                      tileSize * 2 - (y - tileSize * 15) - 1)));
                }
              }
              break;
          case PlayerAction::PORTAL_DOWN:
              for(int y = 0; y < tileSize; ++y)
              {
                for(int x = tileSize * 7; x < tileSize * 9; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x - tileSize * 7,
                                                                                      tileSize - y - 1)));
                }
              }
              break;
          case PlayerAction::PORTAL_RIGHT:
              for(int y = tileSize * 7; y < tileSize * 9; ++y)
              {
                for(int x = tileSize * 15; x < tileSize * 16; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x - tileSize * 15,
                                                                                      tileSize * 2 - (y - tileSize * 7) - 1)));
                }
              }
              break;
          case PlayerAction::PORTAL_LEFT:
              image_x += tileSize;
              for(int y = tileSize * 7; y < tileSize * 9; ++y)
              {
                for(int x = 0; x < tileSize; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x,
                                                                                      tileSize * 2 - (y - tileSize * 7) - 1)));
                }
              }
              break;
          case PlayerAction::WIN:
              for(int y = 0; y < tileSize; ++y)
              {
                for(int x = tileSize * 7; x < tileSize * 9; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), finish_portals.GetPixel(image_x + x - tileSize * 7,
                                                                                             tileSize - y - 1)));
                }
              }
              break;
          default:
              break;
      }
      image_x = (8 - ((falling_count - 1) / 4 % 9)) * playerWidth;
      for(int y = coords.y; y < coords.y + playerHeight; ++y)
      {
        for(int x = coords.x; x < coords.x + playerWidth; ++x)
        {
          screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), Bobby_falling.GetPixel(image_x + x - coords.x,
                                                                playerHeight - (y - coords.y) - 1)));
        }
      }
      falling_count -= 1;
      if ((coords.x >= 2) &&
          (coords.y >= 3) &&
          (coords.x <= screen.Width() - playerWidth - 3) &&
          (coords.y <= screen.Height() - tileSize - 4)) {
          switch (player_action) {
              case PlayerAction::PORTAL_UP:
                  coords.y += 3;
                  break;
              case PlayerAction::PORTAL_DOWN:
              case PlayerAction::WIN:
                  coords.y -= 2;
                  break;
              case PlayerAction::PORTAL_RIGHT:
                  coords.x += 2;
                  break;
              case PlayerAction::PORTAL_LEFT:
                  coords.x -= 2;
                  break;
              default:
                  break;
          }
      }
      if (falling_count == 0) {
          switch (player_action) {
              case PlayerAction::PORTAL_UP:
                  throw ('U');
                  break;
              case PlayerAction::PORTAL_DOWN:
                  throw ('D');
                  break;
              case PlayerAction::PORTAL_RIGHT:
                  throw ('R');
                  break;
              case PlayerAction::PORTAL_LEFT:
                  throw ('L');
                  break;
              case PlayerAction::WIN:
                  throw ('W');
                  break;
              case PlayerAction::WOLF:
                  throw ('A');
                  break;
              default:
                  break;
          }
      }
  } else if (getting_out_count > 0) {
      int image_x;
      int image_y;
      if (getting_out_count > 28) {
          image_x = ((getting_out_count - 30 + 1) / 2) * (tileSize * 2 + 6);
      } else if (getting_out_count > 14) {
          image_x = ((30 - getting_out_count + 1) / 2) * (tileSize * 2 + 6);
      } else {
          image_x = ((getting_out_count + 1) / 2 - 1) * (tileSize * 2 + 6);
      }
      image_y = 0;
      switch (last_portal) {
          case PlayerAction::PORTAL_UP:
              for(int y = 0; y < tileSize; ++y)
              {
                for(int x = tileSize * 7; x < tileSize * 9; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x - tileSize * 7,
                                                                                      tileSize - y - 1)));
                }
              }
              break;
          case PlayerAction::PORTAL_DOWN:
              for(int y = tileSize * 15; y < tileSize * 16; ++y)
              {
                for(int x = tileSize * 7; x < tileSize * 9; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x - tileSize * 7,
                                                                                      tileSize * 2 - (y - tileSize * 15) - 1)));
                }
              }
              break;
          case PlayerAction::PORTAL_RIGHT:
              image_x += tileSize;
              for(int y = tileSize * 7; y < tileSize * 9; ++y)
              {
                for(int x = 0; x < tileSize; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x,
                                                                                      tileSize * 2 - (y - tileSize * 7) - 1)));
                }
              }
              break;
          case PlayerAction::PORTAL_LEFT:
              for(int y = tileSize * 7; y < tileSize * 9; ++y)
              {
                for(int x = tileSize * 15; x < tileSize * 16; ++x)
                {
                  screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), portals.GetPixel(image_x + x - tileSize * 15,
                                                                                      tileSize * 2 - (y - tileSize * 7) - 1)));
                }
              }
              break;
          default:
              break;
      }
      image_x = ((getting_out_count - 1) / 4 % 9) * playerWidth;
      for(int y = coords.y; y < coords.y + playerHeight; ++y)
      {
        for(int x = coords.x; x < coords.x + playerWidth; ++x)
        {
          screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), Bobby_falling.GetPixel(image_x + x - coords.x,
                                                                playerHeight - (y - coords.y) - 1)));
        }
      }
      getting_out_count -= 1;
      switch (last_portal) {
          case PlayerAction::PORTAL_UP:
              coords.y += 2;
              break;
          case PlayerAction::PORTAL_DOWN:
              coords.y -= 2;
              break;
          case PlayerAction::PORTAL_RIGHT:
              coords.x += 2;
              break;
          case PlayerAction::PORTAL_LEFT:
              coords.x -= 2;
              break;
          default:
              break;
      }
      if (getting_out_count == 0) {
          active = true;
          player_action = PlayerAction::MOVE;
      }
  } else if (dying_count > 0) {
      int image_x = (7 - (dying_count / 4 % 8)) * playerWidth;
      for(int y = coords.y; y < coords.y + playerHeight; ++y)
      {
        for(int x = coords.x; x < coords.x + playerWidth; ++x)
        {
          screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), Bobby_dying.GetPixel(image_x + x - coords.x,
                                                                playerHeight - (y - coords.y) - 1)));
        }
      }
      dying_count -= 1;
      if (dying_count == 0) {
          player_action = PlayerAction::DIE;
          throw('F');
      }
  }
}

bool Wolf::Move(Player &player, std::string &room, Image &currentBackground, Image &screen) {
    if (!active) {
        return false;
    }
    
    Point player_coords = player.GetCoords();
    old_coords = coords;
    
    if ((player_coords.x + playerWidth >= home.x - area * tileSize / 2 + tileSize) &&
        (player_coords.x - 1 <= home.x + area * tileSize / 2 + tileSize - 1) &&
        (player_coords.y + playerHeight >= home.y - area * tileSize / 2 + tileSize / 2) &&
        (player_coords.y < home.y + area * tileSize / 2 - 1 + tileSize / 2)) {
        if (coords.x > player_coords.x + playerWidth) {
            coords.x -= move_speed;
            last_dir = MovementDir::LEFT;
        } else if (coords.x + width < player_coords.x) {
            coords.x += move_speed;
            last_dir = MovementDir::RIGHT;
        }
        if (coords.y > player_coords.y + playerHeight) {
            coords.y -= move_speed;
        } else if (coords.y + height < player_coords.y) {
            coords.y += move_speed;
        }
        
        if ((coords.x + width + 1 >= player_coords.x) && (coords.x + width + 1 <= player_coords.x + playerWidth / 2) &&
            (coords.y + height + 1 >= player_coords.y) && (coords.y + height + 1 <= player_coords.y + playerHeight)) {
            coords.x = player_coords.x - width;
            last_dir = MovementDir::RIGHT;
            return true;
        }
        if ((coords.x - 1 >= player_coords.x + playerWidth / 2) && (coords.x - 1 <= player_coords.x + playerWidth) &&
            (coords.y + height + 1 >= player_coords.y) && (coords.y + height + 1 <= player_coords.y + playerHeight)) {
            coords.x = player_coords.x + playerWidth;
            last_dir = MovementDir::LEFT;
            return true;
        }
        if ((coords.y + height + 1 >= player_coords.y) && (coords.y + height + 1 <= player_coords.y + playerHeight / 2) &&
            (coords.x >= player_coords.x - width) && (coords.x <= player_coords.x + playerWidth)) {
            coords.y = player_coords.y - height;
            return true;
        }
        if ((coords.y - 1 >= player_coords.y + playerHeight / 2) && (coords.y - 1 <= player_coords.y + playerHeight) &&
            (coords.x >= player_coords.x - width) && (coords.x <= player_coords.x + playerWidth)) {
            coords.y = player_coords.y + playerWidth;
            return true;
        }
        
        return false;
    }
    
    if ((coords.x != home.x) || (coords.y != home.y)) {
        if (coords.x > home.x + move_speed) {
            coords.x -= move_speed;
            last_dir = MovementDir::LEFT;
        } else if (coords.x < home.x - + move_speed) {
            coords.x += move_speed;
            last_dir = MovementDir::RIGHT;
        }
        if (coords.y > home.y + move_speed) {
            coords.y -= move_speed;
        } else if (coords.y < home.y - move_speed) {
            coords.y += move_speed;
        }
    }
    
    return false;
}

void Wolf::Draw(Image &screen, Image &currentBackground) {
    if (!active && (firing_count == 0)) {
        return;
    }
    
    if ((old_coords.x != coords.x) || (old_coords.y != coords.y)) {
        for(int y = old_coords.y; y < old_coords.y + height; ++y)
        {
          for(int x = old_coords.x; x < old_coords.x + width; ++x)
          {
            screen.PutPixel(x, y, currentBackground.GetPixel(x, y));
          }
        }
        if (switch_image == 3) {
            current_image = (current_image + 1) % 5;
        }
        switch_image = (switch_image + 1) % 4;
    }
    
    if (firing_count > 0) {
        int image_x = ((firing_count - 1) / 4 % 9) * 235;
        if (last_dir == MovementDir::RIGHT) {
            for(int y = coords.y - 15; y < coords.y + 100; ++y)
            {
              for(int x = coords.x; x < coords.x + 235; ++x)
              {
                screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), wolf_firing_right.GetPixel(image_x + x - coords.x,
                                                                                              99 - y + coords.y)));
              }
            }
        } else {
            for(int y = coords.y - 15; y < coords.y + 100; ++y)
            {
              for(int x = coords.x - 96; x < coords.x + 139; ++x)
              {
                screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), wolf_firing_left.GetPixel(image_x + x - coords.x + 96,
                                                                                        99 - y + coords.y)));
              }
            }
        }
        firing_count -= 1;
        if (firing_count == 0) {
            active = true;
        }
        return;
    }
    
    if ((coords.x >= home.x - move_speed) && (coords.x <= home.x + move_speed) &&
        (coords.y >= home.y - move_speed) && (coords.y <= home.y + move_speed)) {
        for(int y = coords.y; y < coords.y + height; ++y)
        {
          for(int x = coords.x; x < coords.x + width; ++x)
          {
            screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), wolf_sleeping.GetPixel(x - coords.x,
                                                                  height - (y - coords.y) - 1)));
          }
        }
    } else if (last_dir == MovementDir::LEFT) {
        int image_x = current_image * width;
        int image_y = 0;
        for(int y = coords.y; y < coords.y + height; ++y)
        {
          for(int x = coords.x; x < coords.x + width; ++x)
          {
            screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), wolf_left.GetPixel(image_x + x - coords.x,
                                                                  height + image_y - (y - coords.y) - 1)));
          }
        }
    } else {
        int image_x = current_image * width;
        int image_y = 0;
        for(int y = coords.y; y < coords.y + height; ++y)
        {
          for(int x = coords.x; x < coords.x + width; ++x)
          {
            screen.PutPixel(x, y, Blend(screen.GetPixel(x, y), wolf_right.GetPixel(image_x + x - coords.x,
                                                                  height + image_y - (y - coords.y) - 1)));
          }
        }
    }
}

void Wolf::EraseWolfAndPlayer(Player &player, Image &screen, Image &currentBackground) {
    if (last_dir == MovementDir::RIGHT) {
        for(int y = coords.y - 15; y < coords.y + 100; ++y)
        {
          for(int x = coords.x; x < coords.x + 235; ++x)
          {
            screen.PutPixel(x, y, currentBackground.GetPixel(x, y));
          }
        }
    } else {
        for(int y = coords.y - 15; y < coords.y + 100; ++y)
        {
          for(int x = coords.x - 96; x < coords.x + 139; ++x)
          {
              screen.PutPixel(x, y, currentBackground.GetPixel(x, y));
          }
        }
    }
    
    Point pos = player.GetCoords();
    for(int y = pos.y; y < pos.y + playerHeight; ++y) {
      for(int x = pos.x; x < pos.x + playerWidth; ++x) {
        screen.PutPixel(x, y, currentBackground.GetPixel(x, y));
      }
    }
                              
}
