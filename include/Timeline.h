#ifndef MUGEN_TIMELINE_H
#define MUGEN_TIMELINE_H

#include "Segment.h"
#include "Texture.h"
#include "Direction.h"


class Timeline {
public:
    Timeline(SDL_Renderer* renderer, TTF_Font* font, ColorTheme theme, int renderedCols, int colWidth, int height, int startingColCount){
        this->theme = theme;
        this->renderer = renderer;
        this->font = font;
        this->renderedCols = renderedCols;
        this->colWidth = colWidth;
        this->width = renderedCols * colWidth;
        this->height = height;
        middleColX = (renderedCols/2) * colWidth - renderedCols/2;
        for (int i = 0; i < 100; i++){
            allSegs.push_back(new Segment(startingColCount, i));
        }
        songSegs.push_back(allSegs.front());
    }

    bool timelineFocused = false;
    int focusedSegmentIndex = 0;
    int focusedColIndex = 0;
    int focusedBitIndex = 0;
    bool editingMode = false;

    TTF_Font* font;
    SDL_Renderer* renderer;
    ColorTheme theme;
    string dashdash = "--";
    SDL_Rect outline;

    int width, height;
    int colWidth = 45;
    int renderedCols;
    int tempo = 150;
    int middleColX;
    vector<Segment*> songSegs;
    vector<Segment*> allSegs;

    Texture bitInstrumentLabel, bitNoteLabel;

    void render(int x, int y) {
        outline = {x, y, width - renderedCols + 1, height - 3};
        SetRenderDrawColor(renderer, theme.inactiveBorder);
        renderLeftUnfocusedSegments(x, y);
        renderRightUnfocusedSegments(x, y);
        SetRenderDrawColor(renderer, theme.foreground);
        SDL_RenderDrawRect(renderer, &outline);
        renderFocusedSegmentLeftSide(x, y);
        renderFocusedSegmentRightSide(x, y);
    }

    void renderLeftUnfocusedSegments(int x, int y){
        int focusedSegmentRenderedColumns = focusedColIndex;
        if (focusedSegmentRenderedColumns > renderedCols/2){
            return;
        }
        if (focusedSegmentIndex > 0){
            int renderedSegmentIndex = focusedSegmentIndex - 1;
            int leftColumnsLeftToRender = renderedCols/2 - focusedColIndex;
            int leftColumnsRendered = 0;

            while (true){
                if (leftColumnsRendered == leftColumnsLeftToRender) break;
                if (leftColumnsRendered && leftColumnsRendered % getSegCols() == 0){
                    if (--renderedSegmentIndex < 0) break;
                }
                int renderedColumnIndex = getSegCols() - 1 - (leftColumnsRendered % getSegCols());
                SDL_Rect colOutline = {x + middleColX - (focusedColIndex * (colWidth - 1)) - (leftColumnsRendered + 1) * (colWidth - 1), y, colWidth, height};

                for (int j = 0; j < 5; j++){
                    auto renderedBit = songSegs.at(renderedSegmentIndex)->cols.at(renderedColumnIndex)->bits[j];
                    SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 3};
                    setBitLabels(renderedBit, false);
                    renderBit(renderedBit, bitOutline);
                }

                leftColumnsRendered++;
            }
        }
    }

    void renderRightUnfocusedSegments(int x, int y){

        int focusedSegmentRenderedColumns = getSegCols() - focusedColIndex;
        if (focusedSegmentRenderedColumns > renderedCols/2){
            return;
        }
        if (focusedSegmentIndex < songSegs.size()-1)
        {
            int rightColumnsLeftToRender = (renderedCols/2)+1- (focusedSegmentRenderedColumns);
            int rightColumnsRendered = 0;
            int renderedSegmentIndex = focusedSegmentIndex + 1;

            while (true){

                if (rightColumnsRendered == rightColumnsLeftToRender) break;
                if (rightColumnsRendered && rightColumnsRendered % getSegCols() == 0){
                    if (++renderedSegmentIndex == songSegs.size()) break;
                }
                int renderedColumnIndex = rightColumnsRendered % getSegCols();
                SDL_Rect colOutline = {x + middleColX + (rightColumnsRendered+1) * (colWidth-1) + (getSegCols() - 1) * (colWidth - 1) - (focusedColIndex * (colWidth - 1)), y, colWidth, height};

                for (int j = 0; j < 5; j++) {
                    SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 3};
                    auto renderedBit = songSegs.at(renderedSegmentIndex)->cols.at(renderedColumnIndex)->bits[j];
                    setBitLabels(renderedBit, false);
                    renderBit(renderedBit, bitOutline);
                }

                rightColumnsRendered++;
            }
        }
    }


    void renderFocusedSegmentLeftSide(int x, int y){
        int columnsLeftToRender = std::min(focusedColIndex+1, renderedCols/2);
        int columnsRendered = 0;

        for (int i = focusedColIndex - 1; i >= 0; i--){
            if (columnsRendered == columnsLeftToRender) break;
            SDL_Rect colOutline = {x + middleColX + (i * (colWidth - 1)) - (focusedColIndex * (colWidth - 1)), y, colWidth, height};
            for (int j = 0; j < 5; j++){
                auto renderedBit = songSegs.at(focusedSegmentIndex)->cols.at(i)->bits[j];
                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 3};
                bool bitFocused = (focusedColIndex == i && focusedBitIndex == j);
                if (timelineFocused && bitFocused){
                    renderBitBackground(&bitOutline);
                }
                setBitLabels(renderedBit, true, bitFocused);
                renderBit(renderedBit, bitOutline);
            }
            columnsRendered++;
        }
    }

    void renderFocusedSegmentRightSide(int x, int y){
        int columnsLeftToRender = std::min((getSegCols() - focusedColIndex), (renderedCols / 2 + 1));
        int columnsRendered = 0;

        for (int i = focusedColIndex; i < getSegCols(); i++){
            if (columnsRendered == columnsLeftToRender) break;
            SDL_Rect colOutline = {x + middleColX + (i * (colWidth - 1)) - (focusedColIndex * (colWidth - 1)), y, colWidth, height};
            for (int j = 0; j < 5; j++){
                auto renderedBit = songSegs.at(focusedSegmentIndex)->cols.at(i)->bits[j];

                SDL_Rect bitOutline = {colOutline.x, colOutline.y + (colOutline.h / 5) * j, colWidth, colOutline.h / 5 - 3};
                bool bitFocused = (focusedColIndex == i && focusedBitIndex == j);
                if (timelineFocused && bitFocused){
                    renderBitBackground(&bitOutline);
                }

                setBitLabels(renderedBit, true, bitFocused);
                renderBit(renderedBit, bitOutline);

            }
            columnsRendered++;
        }
    }

    void renderBit(const Bit *renderedBit, SDL_Rect &bitOutline) {
        if (renderedBit != nullptr && renderedBit->holdDuration > 0){
            SDL_RenderDrawLine(renderer, bitOutline.x, bitOutline.y, // gora
                               bitOutline.x + bitOutline.w, bitOutline.y);
            SDL_RenderDrawLine(renderer, bitOutline.x, bitOutline.y + bitOutline.h-1, // dol
                               bitOutline.x + bitOutline.w, bitOutline.y + bitOutline.h-1);
            if (renderedBit->holdSection == 0){
                SDL_RenderDrawLine(renderer, bitOutline.x, bitOutline.y, // lewo
                                   bitOutline.x, bitOutline.y + bitOutline.h);
                renderBitLabels(&bitOutline);
            } else if (renderedBit->holdSection == renderedBit->holdDuration) {
                SDL_RenderDrawLine(renderer, bitOutline.x + bitOutline.w, bitOutline.y,  // prawo
                                   bitOutline.x + bitOutline.w, bitOutline.y + bitOutline.h);
                renderBitLabels(&bitOutline);
            }
        } else {
            SDL_RenderDrawRect(renderer, &bitOutline);
            renderBitLabels(&bitOutline);
        }
    }

    void renderBitBackground(SDL_Rect* bitOutline) const{
        if (!editingMode){
            SetRenderDrawColor(renderer, theme.focusedBit);
        }
        SDL_RenderFillRect(renderer, bitOutline);
        if (!editingMode){
            SetRenderDrawColor(renderer, theme.foreground);
        }
    }

    void setBitLabels(Bit* renderedBit, bool segmentFocused, bool bitFocused = false){
        if (segmentFocused) {
            if (renderedBit != nullptr){
                if (bitFocused && editingMode){
                    setStrColorToBackground(&bitNoteLabel, freqToSymbol(renderedBit->note.frequency));
                    setStrColorToBackground(&bitInstrumentLabel, getTwoDigitString(renderedBit->instrument->index));
                } else {
                    setStrColorToForeground(&bitNoteLabel, freqToSymbol(renderedBit->note.frequency));
                    setStrColorToForeground(&bitInstrumentLabel, getTwoDigitString(renderedBit->instrument->index));
                }
            } else {
                if (bitFocused && editingMode){
                    setStrColorToBackground(&bitNoteLabel, dashdash);
                    setStrColorToBackground(&bitInstrumentLabel, dashdash);
                } else {
                    setStrColorToForeground(&bitNoteLabel, dashdash);
                    setStrColorToForeground(&bitInstrumentLabel, dashdash);
                }
            }
        } else {
            if (renderedBit != nullptr){
                setStrColorToInactive(&bitNoteLabel, freqToSymbol(renderedBit->note.frequency));
                setStrColorToInactive(&bitInstrumentLabel, getTwoDigitString(renderedBit->instrument->index));
            } else {
                setStrColorToInactive(&bitNoteLabel, dashdash);
                setStrColorToInactive(&bitInstrumentLabel, dashdash);
            }
        }
    }

    void renderBitLabels(SDL_Rect* bitOutline) {
        bitNoteLabel.render(bitOutline->x + bitOutline->w / 2 - bitNoteLabel.w / 2, bitOutline->y + bitOutline->h / 2 - bitNoteLabel.h + 2);
        bitInstrumentLabel.render(bitOutline->x + bitOutline->w / 2 - bitInstrumentLabel.w / 2, bitOutline->y + bitOutline->h / 2 + 2);
    }

    void setStrColorToBackground(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, theme.background, font);
    }

    void setStrColorToInactive(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, theme.inactiveText, font);
    }

    void setStrColorToForeground(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, theme.foreground, font);
    }

    void move(Direction direction){
        switch (direction) {
            case Direction::UP:
                if (focusedBitIndex > 0)
                    focusedBitIndex--;
                break;
            case Direction::DOWN:
                if (focusedBitIndex < 4)
                    focusedBitIndex++;
                break;
            case Direction::LEFT:
                if (focusedColIndex > 0){
                    focusedColIndex--;
                } else if (focusedSegmentIndex > 0) {
                    focusedSegmentIndex--;
                    focusedColIndex = getSegCols() - 1;
                }
                break;
            case Direction::RIGHT:
                if (focusedColIndex < getSegCols() - 1){
                    focusedColIndex++;
                }
                else if (focusedSegmentIndex < songSegs.size() - 1){
                    focusedSegmentIndex++;
                    focusedColIndex = 0;
                }
                break;
        }
    }

    int getSegCols(){
        return static_cast<int>(allSegs.front()->cols.size());
    }

    static string freqToSymbol(double frequency){
        int midi = freqToMidi(frequency);
        return midiToSymbol(midi);
    }

    static string midiToSymbol(int midinote){

        int octave = midinote / 12;
        int note = midinote % 12;

        switch (note){
            case 0:
                return "C" + to_string(octave);
            case 1:
                return "C#" + to_string(octave);
            case 2:
                return "D" + to_string(octave);
            case 3:
                return "D#" + to_string(octave);
            case 4:
                return "E" + to_string(octave);
            case 5:
                return "F" + to_string(octave);
            case 6:
                return "F#" + to_string(octave);
            case 7:
                return "G" + to_string(octave);
            case 8:
                return "G#" + to_string(octave);
            case 9:
                return "A" + to_string(octave);
            case 10:
                return "A#" + to_string(octave);
            case 11:
                return "B" + to_string(octave);
            default:
                return "NaN";
        }
    }

};


#endif //MUGEN_TIMELINE_H
