#include "engine.h"
#include <iostream>
#include "sdl2/SDL.h"

namespace rendeer
{
    Engine *Engine::sInstance = nullptr;
    Engine::Engine() : mIsRunning(false)
    {
        if (sInstance == nullptr)
            sInstance = this;
    }

    void Engine::Create()
    {
        Engine();
    }

    void Engine::Run()
    {
        if (sInstance->Initialize())
        {
            while (sInstance->mIsRunning)
            {
                sInstance->mWindow.PumpEvents();
            }

            sInstance->Shutdown();
        }
    }

    bool Engine::Initialize()
    {
        bool ret = false;
        
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout<<"Error initializing SDL2: " << SDL_GetError() << std::endl;
        }

        SDL_version version;
        SDL_VERSION(&version);
        std::cout << "SDL " << (int32_t) version.major << "." << (int32_t) version.minor << "." << (int32_t) version.patch << std::endl;

        if (mWindow.Create())
        {
            ret = true;
            mIsRunning = true;
        }

        if (!ret)
        {
            std::cout<<"Error while initializing Engine. Shutting down." << std::endl;
            Shutdown();
        }

        std::cout<<"Initialized successfully";

        return ret;
    }

    void Engine::Shutdown()
    {
        mWindow.Shutdown();
        SDL_Quit();
    }
}