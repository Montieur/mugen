#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include "include/MusicBox.h"
#include "include/MainWindow.h"

int main()
{
    assets_dir = std::filesystem::is_directory("./assets/") ? "./assets/" : "../assets/";
    printf("%s", assets_dir.c_str());
    Config config;
    config.load();
    MusicBox musicBox(config);
    MainWindow mainWindow(&musicBox, config);
    bool lastKeyState[128];
    int keyPressState[128] = {0};
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < 128; i++)
        lastKeyState[i] = state[i];

    musicBox.startPlaying();
    while (!mainWindow.quit){
        SDL_PumpEvents();
        state = SDL_GetKeyboardState(nullptr);
        mainWindow.handleKeyPress(state, lastKeyState, keyPressState);
        for (int i = 0; i < 128; i++)
            lastKeyState[i] = state[i];
        mainWindow.render();
    }

    exit(0);
}
