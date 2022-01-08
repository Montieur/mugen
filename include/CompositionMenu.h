#ifndef MUGEN_COMPOSITIONMENU_H
#define MUGEN_COMPOSITIONMENU_H


#include "Texture.h"
#include "Direction.h"
#include "ItemList.h"
#include "Timeline.h"
#include <vector>
#include "MainWindow.h"
#include "SelectControl.h"
#include "SegmentManager.h"

class MainWindow;

class CompositionMenu {

public:
    CompositionMenu(MainWindow* mainWindow);

    SDL_Renderer* renderer;
    SDL_Color textColor;
    MainWindow* window;
    MusicBox* musicBox;
    ItemList* instrumentList;
    Timeline* timeline;
    SegmentManager* segmentManager;

    SelectControl tempoSelector, segmentsSelector, colsSelector;

    const static int controlMatrixRows = 3, controlMatrixCols = 1;

    int focusedControlRow = 0, focusedControlCol = 0;
    Control* controlArray[controlMatrixRows][controlMatrixCols] = {
            {&segmentsSelector},
            {&colsSelector},
            {&tempoSelector}
    };

    bool isTimelineFocused = false;

    vector<Bit*> bitsPlayed;

    Texture instrumentListLabel,
            segmentsLabel, colsLabel, tempoLabel,
            segmentsValue, colsValue, tempoValue;

    void render();

    void setTextTexture(Texture *texture, const string& text) const;

    void setTextTexture(Texture *texture, const string& text, TTF_Font *font) const;

    void loadTextures();

    void updateTextures();

    void init();

    int xByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int xByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;
    int yByPercent(Texture *texture, double percent, Alignment align = CENTER) const;
    int yByPercent(SDL_Rect *rect, double percent, Alignment align = CENTER) const;

    void loadControls();

    void addSegment();

    void removeSegment();

    void addColumn();

    void removeColumn();

    void handleKeyPress(SDL_Keycode key);

    Control* getFocusedControl();

    void changeControlFocus(Direction direction);

    void selectFocusedControl();

    void loadExampleBits();

    void playbackTimeline();

    void switchTimelineFocus();
};


#endif //MUGEN_COMPOSITIONMENU_H
