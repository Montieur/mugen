#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <thread>
#include "include/MusicBox.h"
#include "include/MainWindow.h"

int main() {

    MusicBox* musicBox = new MusicBox();
    printf("test1\n");
    MainWindow* mainWindow = new MainWindow(musicBox);
    printf("test2\n");
    bool lastKeyState[128];
    int keyPressState[128] = {0};
    const Uint8* state;
    state = SDL_GetKeyboardState(nullptr);
    printf("test3\n");
    for (int i = 0; i < 128; i++)
        lastKeyState[i] = state[i];

    musicBox->startPlaying();
    printf("test4\n");
    while (!mainWindow->quit){
        SDL_PumpEvents();
        printf("test5\n");
        int keyboardSize = 20;
        state = SDL_GetKeyboardState(&keyboardSize);
        printf("test6\n");
        mainWindow->handleKeyPress(state, lastKeyState, keyPressState);
        printf("test7\n");
        for (int i = 0; i < 128; i++)
            lastKeyState[i] = state[i];
        mainWindow->render();
        printf("test8\n");
    }
    delete mainWindow;
    delete musicBox;
    exit(0);
}
