#pragma once

#include "core/window.h"
#include "rendermanager.h"

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
        RenderManager* mRenderManager;

        Engine();
        bool Initialize();
        void Shutdown();
    };
}