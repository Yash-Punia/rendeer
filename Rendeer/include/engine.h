#pragma once

#include "core/window.h"
#include "rendermanager.h"
#include "sdl2/SDL.h"
#include "glm/glm.hpp"

namespace rendeer
{
    class Engine
    {
    public:
        ~Engine() {}
        
        static void Create();
        static void Run();

        void Update();
        void Render();

        inline static void Quit() { sInstance->mIsRunning = false; }
        inline static float GetTicks() { return (float) SDL_GetTicks() / 1000; }
        inline static glm::vec2 GetWindowSize() { return sInstance->mWindow.GetWindowSize(); }
    private:
        static Engine *sInstance;
        float mLastFrame, mDeltaTime;
        bool mIsRunning;
        Window mWindow;
        RenderManager* mRenderManager;

        Engine();
        bool Initialize();
        void Shutdown();
    };
}