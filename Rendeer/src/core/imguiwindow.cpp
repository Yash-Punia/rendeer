#include "core/imguiwindow.h"
#include "engine.h"
#include "sdl2/SDL.h"

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "external/imgui/imgui_impl_sdl.h"

namespace rendeer
{
    void ImguiWindow::Create()
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        auto& window = Engine::GetWindow();
        ImGui_ImplSDL2_InitForOpenGL(window.GetSDLWindow(), window.GetGLContext());
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImguiWindow::HandleSDLEvent(SDL_Event& e)
    {
        ImGui_ImplSDL2_ProcessEvent(&e);
    }

    void ImguiWindow::BeginRender()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(Engine::GetWindow().GetSDLWindow());
        ImGui::NewFrame();
    }

    void ImguiWindow::EndRender()
    {
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool ImguiWindow::WantCaptureMouse()
    {
        return ImGui::GetIO().WantCaptureMouse; 
    }

    bool ImguiWindow::WantCaptureKeyboard()
    {
        return ImGui::GetIO().WantCaptureKeyboard; 
    }

    void ImguiWindow::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
} // namespace rendeer
