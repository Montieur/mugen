#ifndef MUGEN_SEGMENTMANAGER_H
#define MUGEN_SEGMENTMANAGER_H

#include "Timeline.h"

class SegmentManager {
public:
    SegmentManager(Timeline* timeline, TTF_Font* font, int width, int height, int visibleItems){
        this->timeline = timeline;
        this->font = font;
        this->width = width;
        this->height = height;
        this->itemHeight = height / visibleItems;
        this->renderer = timeline->renderer;
        isFocused = false;
    }

    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Rect outline;
    Timeline* timeline;
    int width, height;
    int itemWidth, itemHeight;
    int visibleItems;
    bool isFocused;

    Texture segIndex, segNumber;

    void render(int x, int y){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        outline = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &outline);

        for (int i = timeline->focusedSegmentIndex - 2; i < timeline->focusedSegmentIndex + 3; i++) {
            if (i == timeline->songSegs.size()) break;
            if (i < 0) continue;
            SDL_Rect itemContainer = {x, y + itemHeight * 2 + itemHeight * (i - timeline->focusedSegmentIndex), width, itemHeight};

            if (i == timeline->focusedSegmentIndex){
                if (isFocused){
                    SDL_RenderFillRect(renderer, &itemContainer);
                    setBlackStr(&segIndex, getTwoDigitString(i));
                    setBlackStr(&segNumber, getTwoDigitString(timeline->songSegs.at(i)->index));
                }
                else {
                    SDL_RenderDrawRect(renderer, &itemContainer);
                    setWhiteStr(&segIndex, getTwoDigitString(i));
                    setWhiteStr(&segNumber, getTwoDigitString(timeline->songSegs.at(i)->index));
                }
            } else {
                setWhiteStr(&segIndex, getTwoDigitString(i));
                setWhiteStr(&segNumber, getTwoDigitString(timeline->songSegs.at(i)->index));
            }

            segIndex.render(itemContainer.x + itemContainer.w / 4 - segIndex.w / 2, itemContainer.y + itemContainer.h / 2 - segIndex.h / 2 + 1);
            segNumber.render(itemContainer.x + itemContainer.w * 3 / 4 - segNumber.w / 2, itemContainer.y + itemContainer.h / 2 - segIndex.h / 2 + 1);
        }
    }

    void setBlackStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, timeline->black, font);
    }

    void setWhiteStr(Texture* texture, const string& text) const{
        texture->loadFromText(renderer, text, timeline->white, font);
    }

    void moveUp(){
        if (timeline->focusedSegmentIndex > 0){
            timeline->focusedSegmentIndex--;
        }
    }

    void moveDown(){
        if (timeline->focusedSegmentIndex < timeline->songSegs.size()-1){
            timeline->focusedSegmentIndex++;
        }
    }

    void segmentUp(){
        if (timeline->songSegs.at(timeline->focusedSegmentIndex)->index < timeline->allSegs.size()-1){
            timeline->songSegs.at(timeline->focusedSegmentIndex) = timeline->allSegs.at(timeline->songSegs.at(timeline->focusedSegmentIndex)->index + 1);
        }
    }

    void segmentDown(){
        if (timeline->songSegs.at(timeline->focusedSegmentIndex)->index > 0){
            timeline->songSegs.at(timeline->focusedSegmentIndex) = timeline->allSegs.at(timeline->songSegs.at(timeline->focusedSegmentIndex)->index - 1);
        }
    }

};


#endif //MUGEN_SEGMENTMANAGER_H
