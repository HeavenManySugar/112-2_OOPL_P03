#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char **) {
    auto context = Core::Context::GetInstance();
    App app;
    constexpr unsigned int MAIN_FPS_CAP = 60;
    constexpr double frameTime =
        MAIN_FPS_CAP != 0 ? 1000 / static_cast<double>(MAIN_FPS_CAP) : 0;

    auto a = Util::Time::GetElapsedTimeMs(), b = Util::Time::GetElapsedTimeMs(),
         delta = a - b;

    while (!context->GetExit()) {
        a = Util::Time::GetElapsedTimeMs();
        delta = a - b;
        if (delta > frameTime) {
            context->Setup();

            switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start(context);
                break;

            case App::State::UPDATE:
                app.Update();
                break;

            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
            }

            b = a;
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            context->Update();
        }
    }
    return 0;
}
