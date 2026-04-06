#include "input/InputHandler.hpp"

#include <cstring>

InputHandler::InputHandler()
{
  current_.fill(0);
  just_pressed_.fill(0);
}

void InputHandler::update()
{
  // Drain event queue — captures every keypress even if already released
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      quit_ = true;
    if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        quit_ = true;
      // Record press regardless of how short it was

      just_pressed_[event.key.keysym.scancode] = 1;
    }
  }

  // Snapshot held state from SDL
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  std::memcpy(current_.data(), state, KEY_COUNT);
}

bool InputHandler::isHeld(SDL_Scancode key) const
{
  return current_[key] != 0;
}

bool InputHandler::justPressed(SDL_Scancode key) const
{
  return just_pressed_[key] != 0;
}

void InputHandler::clearPressed()
{
  just_pressed_.fill(0);
}