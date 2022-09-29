#pragma once
#include "sdl2/SDL.h"

namespace rendeer::input
{
    class Mouse
    {
    public:
        static void Update();
        static void UpdateScroll(float, float);

        inline static int X() { return x; }
        inline static int Y() { return y; }
        inline static int DX() { return (x - xLast); }
        inline static int DY() { return (y - yLast); }
        inline static float ScrollX() { return scrollX; }
        inline static float ScrollY() { return scrollY; }
        
    private:
        static int x, xLast;
        static int y, yLast;
        static float scrollX, scrollY;
    };
} // namespace rendeer::input
