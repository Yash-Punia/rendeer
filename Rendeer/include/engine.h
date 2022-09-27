#pragma once

#include "core/window.h"
#include "rendermanager.h"
#include "sdl2/SDL.h"

namespace rendeer
{
    class Engine
    {
    public:
        ~Engine() {}
        
        static void Create();
        static void Run();
        inline static void Quit() { sInstance->mIsRunning = false; }
        inline static float GetTicks() { return (float) SDL_GetTicks(); }
    private:
        static Engine *sInstance;
        bool mIsRunning;
        Window mWindow;
        RenderManager* mRenderManager;

        Engine();
        bool Initialize();
        void Shutdown();
    };
}