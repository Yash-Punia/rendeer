#pragma once

#include "core/window.h"

namespace rendeer
{
    class Engine
    {
    public:
        ~Engine() {}
        
        static void Create();
        static void Run();
        inline static void Quit() { sInstance->mIsRunning = false; }
    private:
        static Engine *sInstance;
        bool mIsRunning;
        Window mWindow;

        Engine();
        bool Initialize();
        void Shutdown();
    };
}