#include "input/keyboard.h"
#include "sdl2/SDL.h"

namespace rendeer::input
{
    std::array<bool, Keyboard::KeyCount> Keyboard::keys;
    std::array<bool, Keyboard::KeyCount> Keyboard::keysLast;

    void Keyboard::Initialize()
    {
        std::fill(keys.begin(), keys.end(), false);
        std::fill(keysLast.begin(), keysLast.end(), false);
    }

    void Keyboard::Update()
    {
        std::fill(keysLast.begin(), keysLast.end(), false);

        keysLast = keys;

        const Uint8* state = SDL_GetKeyboardState(nullptr);
        for (int i = RENDEER_INPUT_KEY_FIRST; i < KeyCount; i++)
            keys[i] = state[i];        
    }

    bool Keyboard::GetKey(int key)
    {
        if (key >= RENDEER_INPUT_KEY_FIRST && key < KeyCount)
            return keys[key];
        return false;
    }

    bool Keyboard::GetKeyDown(int key)
    {
        if (key >= RENDEER_INPUT_KEY_FIRST && key < KeyCount)
            return keys[key] && !keysLast[key];
        return false;
    }

    bool Keyboard::GetKeyUp(int key)
    {
        if (key >= RENDEER_INPUT_KEY_FIRST && key < KeyCount)
            return !keys[key] && keysLast[key];
        return false;
    }
} // namespace rendeer::input
