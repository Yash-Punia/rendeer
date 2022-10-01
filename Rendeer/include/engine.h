#pragma once

#include "core/window.h"
#include "app.h"
#include "sdl2/SDL.h"
#include "glm/glm.hpp"

namespace rendeer
{
    class Engine
    {
    public:
        ~Engine() {}
        
        static void Create(App* app);
        static void Run();

        void Update();
        void Render();

        inline static void Quit() { sInstance->mIsRunning = false; }
        inline static float GetTicks() { return (float) SDL_GetTicks() / 1000; }
        inline static Window& GetWindow() { return sInstance->mWindow; }
        inline static App* GetApp() { return mApp; }
    private:
        static Engine *sInstance;
        static App* mApp;
        float mLastFrame, mDeltaTime;
        bool mIsRunning;
        Window mWindow;

        Engine();
        bool Initialize();
        void Shutdown();
    };
}