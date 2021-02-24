#include "Player.h"
Image player_img("../../Stray Cat/resources/player.png");
Image static_background("../../Stray Cat/resources/grass-background.png");

bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
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
