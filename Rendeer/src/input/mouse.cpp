#include "input/mouse.h"
#include "sdl2/SDL.h"

namespace rendeer::input
{
    int Mouse::x{0}, Mouse::xLast{0};
    int Mouse::y{0}, Mouse::yLast{0};
    float Mouse::scrollX{0.0f}, Mouse::scrollY{0.0f};

    void Mouse::Update()
    {
        xLast = x;
        yLast = y;
        Uint32 state = SDL_GetMouseState(&x, &y);
    }

    void Mouse::UpdateScroll(float x, float y)
    {
        scrollX = x;
        scrollY = y;
    }
} // namespace rendeer::input
