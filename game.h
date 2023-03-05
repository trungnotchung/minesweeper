#ifndef _GAME_H
#define _GAME_H
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <queue>
#include <iostream>
#include "ltexture.h"
#include "lbutton.h"
//Starts up SDL and creates window

const int SCREEN_WIDTH = 488;
const int SCREEN_HEIGHT = 488;
const int SIZE = 8;
const int CNT_BOMB = 10;
const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

enum LCellType
{
    EMPTY = 0,
    NUMONE = 1,
    NUMTWO = 2,
    NUMTHREE = 3,
    NUMFOUR = 4,
    NUMFIVE = 5,
    NUMSIX = 6,
    NUMSEVEN = 7,
    NUMEIGHT = 8, 
    BOMB = 9,
    DEFAULT = 10,
    FLAG = 11,
    TOTAL_CELL = 12
};

class LGame
{
    public:

        LGame();

        ~LGame();

        bool init();

        bool loadMedia(LTexture &curTexture, const std::string &path);

        bool loadString(LTexture &curTexture, const std::string &path, const std::string &str);

        void close();

        void loadGame();

        int getWidth();

        int getHeight();

        void setPosition();

        void generateCellType(SDL_Point mouseClick);

        void playGame();

        void processMouseClick(SDL_Point mouseClick, bool rightClick);

        bool inside(int x, int y);

        SDL_Point lastestMousePosition(SDL_Event* e);


    private:
        SDL_Window *gWindow;
        SDL_Renderer *gRenderer;
        LTexture gCell[TOTAL_CELL];
        LTexture curGrid[SIZE][SIZE];
        LTexture gameWin, gameLose;
        SDL_Point topLeftPositionCell[SIZE][SIZE];
        int cellType[SIZE][SIZE];
        bool haveBomb[SIZE][SIZE];
        bool haveFlag[SIZE][SIZE];
        bool isHide[SIZE][SIZE];
        bool winGame, endGame, generated;
        int numOpenCell, remainFlag, cntFrame;
        std::queue<std::pair<int, int> > q;;
};
#endif