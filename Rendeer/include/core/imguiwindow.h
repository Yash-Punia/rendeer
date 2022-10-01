#pragma once

typedef union SDL_Event;


namespace rendeer
{
    class ImguiWindow
    {
    public:
        ImguiWindow() {}
        ~ImguiWindow() {} 

        void Create();
        void Shutdown();

        void HandleSDLEvent(SDL_Event& e);
        bool WantCaptureMouse();
        bool WantCaptureKeyboard();

        void BeginRender();
        void EndRender();

    };
} // namespace rendeer
