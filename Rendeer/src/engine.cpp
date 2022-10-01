#include "engine.h"
#include "app.h"
#include "sdl2/SDL.h"
#include <iostream>

namespace rendeer
{
    Engine *Engine::sInstance = nullptr;
    App *Engine::mApp = nullptr;

    Engine::Engine() : mIsRunning(false), mDeltaTime(0), mLastFrame(0)
    {
    }

    void Engine::Create(App *app)
    {
        if (sInstance == nullptr)
        {
            sInstance = new Engine();
            mApp = app;
        }
    }

    void Engine::Run()
    {
        if (sInstance->Initialize())
        {
            while (sInstance->mIsRunning)
            {
                sInstance->Render();
                sInstance->Update();
            }
            std::cout << "Shutting down!\n";
            sInstance->Shutdown();
        }
    }

    bool Engine::Initialize()
    {
        bool ret = false;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "Error initializing SDL2: " << SDL_GetError() << std::endl;
        }

        SDL_version version;
        SDL_VERSION(&version);
        std::cout << "SDL " << (int32_t)version.major << "." << (int32_t)version.minor << "." << (int32_t)version.patch << std::endl;

        if (mWindow.Create())
        {
            ret = true;
            mIsRunning = true;
            mApp->Initialize();
        }

        if (!ret)
        {
            std::cout << "Error while initializing Engine. Shutting down." << std::endl;
            Shutdown();
        }

        std::cout << "Initialized successfully\n";

        return ret;
    }

    void Engine::Render()
    {
        mWindow.BeginRender();
        mApp->Render();
        mWindow.EndRender();

        // mApp->UI_Render();
    }

    void Engine::Update()
    {
        float currentFrame = GetTicks();
        mDeltaTime = currentFrame - mLastFrame;
        mLastFrame = currentFrame;
        mWindow.PumpEvents();

        mApp->Update(mDeltaTime);
    }

    void Engine::Shutdown()
    {
        mApp->Shutdown();
        mWindow.Shutdown();
        SDL_Quit();
    }
}