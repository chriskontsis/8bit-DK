#pragma once

#include <SDL2/SDL.h>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

// static geometry, no velocity, no v dispatch
struct Platform
{
  float     x, y, width, height;
  float     conveyor_speed = 0.0f;  // px/s + pushes right, - pushes left
  SDL_Color color{180, 120, 60, 255};
  float     slope = 0.0f;  // visual tilt

  SDL_FRect rect() const { return {x, y, width, height}; }
  void render(SDL_Renderer* r) const
  {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    if (slope == 0.0f)
    {
      SDL_FRect rc = rect();
      SDL_RenderFillRectF(r, &rc);
    }
    else
    {
      // Draw as slanted parallelogram. new strip each time y - offset steps by 1px
      int   prev_off = 0;
      float strip_start = 0.0f;
      for (float px = 1.0f; px <= width + 1.0f; ++px)
      {
        int cur_off = static_cast<int>(slope * px);
        if (cur_off != prev_off || px > width)
        {
          SDL_FRect strip = {x + strip_start, y + prev_off, px - strip_start, height};
          SDL_RenderFillRectF(r, &strip);
          strip_start = px;
          prev_off = cur_off;
        }
      }
    }
    // conveyor stripe - visual indicator when belt is active
    if (conveyor_speed != 0.0f)
    {
      SDL_SetRenderDrawColor(r, 255, 220, 50, 180);
      for (float sx = x; sx < x + width; sx += 24.0f)
      {
        SDL_FRect stripe = {sx, y + 4, 12, height - 8};
        SDL_RenderFillRectF(r, &stripe);
      }
    }
  }
};