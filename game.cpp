#include "game.h"
#include "lrandom.h"
#include "ltexture.h"

void LGame::loadGame()
{
    //init program
    init();

    //loadMedia
    loadMedia(gCell[EMPTY], "picture/empty.png");
    // loadMedia(gCell[DEFAULT], "picture/default.png");
    // loadMedia(gCell[NUMONE], "picture/one.png");
    // loadMedia(gCell[NUMTWO], "picture/two.png");
    // loadMedia(gCell[NUMTHREE], "picture/three.png");
    // loadMedia(gCell[NUMFOUR], "picture/four.png");
    // loadMedia(gCell[NUMFIVE], "picture/five.png");
    // loadMedia(gCell[NUMSIX], "picture/six.png");
    // loadMedia(gCell[NUMSEVEN], "picture/seven.png");
    // loadMedia(gCell[NUMEIGHT], "picture/eight.png");
    // loadMedia(gCell[BOMB], "picture/bomb.png");
    // loadMedia(gCell[FLAG], "picture/flag.png");

    //loadString
    // loadString(gameWin, "lazy.ttf", "You win !!!");
    // loadString(gameLose, "lazy.ttf", "You lose !!!");

    //set coordinate for each cell
    // setPosition();
}

void LGame::playGame()
{
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            SDL_Point mouseClick = lastestMousePosition(&e);

            //generate cell state
            if(mouseClick.x != -1 || mouseClick.y != -1)
            {
                if(!generated)
                {
                    generated = true;
                    generateCellType(mouseClick);
                }
                processMouseClick(mouseClick, (e.button.button == SDL_BUTTON_RIGHT));
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render buttons
        for(int i=0; i<SIZE; ++i)
            for(int j=0; j<SIZE; ++j)
            {
                if(haveFlag[i][j] == true)
                    gCell[FLAG].render(gRenderer, topLeftPositionCell[i][j].x, topLeftPositionCell[i][j].y);
                else if(isHide[i][j] == false)
                    curGrid[i][j].render(gRenderer, topLeftPositionCell[i][j].x, topLeftPositionCell[i][j].y);
                else 
                    gCell[DEFAULT].render(gRenderer, topLeftPositionCell[i][j].x, topLeftPositionCell[i][j].y);
            }

        //Update screen
        SDL_RenderPresent( gRenderer );

        // if(endgame)
        //     quit = true;
    }
}

bool LGame::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void LGame::setPosition()
{
    for(int i=0; i<SIZE; ++i)
    {
        for(int j=0; j<SIZE; ++j)
            topLeftPositionCell[i][j].x = i * 61, topLeftPositionCell[i][j].y = j * 61;
    }
}

//creat game state
void LGame::generateCellType(SDL_Point mouseClick)
{
    srand(time(NULL));

    for(int x=0; x<SIZE; ++x)
        for(int y=0; y<SIZE; ++y)
            cellType[x][y] = EMPTY;

    for(int i=1; i<=CNT_BOMB; ++i)
    {
        while(true)
        {
            int x = randNum(0, SIZE-1);
            int y = randNum(0, SIZE-1);
            if(haveBomb[x][y]) 
                continue;
            if(x == mouseClick.x && y == mouseClick.y)
                continue;
            haveBomb[x][y] = true;
            cellType[x][y] = BOMB;
            break;
        }

    }

    for(int x=0; x<SIZE; ++x)
        for(int y=0; y<SIZE; ++y)
        {
            if(cellType[x][y] != BOMB)
                continue;
            for(int dir = 0; dir < 8; ++dir)
            {
                int u = x + dx[dir], v = y + dy[dir];
                if(inside(u, v) && cellType[u][v] != BOMB)  
                    cellType[u][v]++;
            }
        }

    for(int i=0; i<SIZE; ++i)
        for(int j=0; j<SIZE; ++j)
            curGrid[i][j] = gCell[cellType[i][j]];
}

// load string texture
bool LGame::loadString(LTexture &curTexture, const std::string &path, const std::string &str)
{
    bool success = true;

    if(!curTexture.loadString(gRenderer, path, str))
    {
        printf("Failed to load string texture\n");
        success = false;
    }

    return success;
}

bool LGame::loadMedia(LTexture &curTexture, const std::string &path)
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if( !curTexture.loadFromFile(path, gRenderer) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	return success;
}

int LGame::getWidth()
{
    return gCell[DEFAULT].getWidth();
}

int LGame::getHeight()
{
    return gCell[DEFAULT].getHeight();
}

bool LGame::inside(int x, int y)
{
    return (0 <= x && x < SIZE && 0 <= y && y < SIZE);
}

SDL_Point LGame::lastestMousePosition(SDL_Event* e)
{
    SDL_Point res;
    int x = -1, y = -1;
    res.x = -1, res.y = -1;
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

			//Set mouse over sprite
        if( e->type == SDL_MOUSEBUTTONDOWN)
        {
            for(int i=0; i<SIZE; ++i)
            {
                for(int j=0; j<SIZE; ++j)
                {
                    if(topLeftPositionCell[i][j].x <= x && x <= topLeftPositionCell[i][j].x + 60)
                    {
                        if(topLeftPositionCell[i][j].y <= y && y <= topLeftPositionCell[i][j].y + 60)
                        {
                            res.x = i, res.y = j;
                            return res;
                        }
                    }
                }
            }
        }
	}
    return res;
}

//get position of clicked cell
void LGame::processMouseClick(SDL_Point mouseClick, bool rightClick)
{
    int x = mouseClick.x, y = mouseClick.y;
    if(rightClick)
    {
        if(haveFlag[x][y] || isHide[x][y] == false || remainFlag == 0)
            return;
        haveFlag[x][y] = true;
        --remainFlag;
        return;
    }

    // std::cout << mouseClick.x << " " << mouseClick.y << '\n';
    if(isHide[x][y] == false)
        return;
    isHide[x][y] = false;
    haveFlag[x][y] = false;
    if(haveBomb[x][y])
    {
        endgame = true;
        return;
    }
    if(cellType[x][y] != EMPTY)
    {
        ++numOpenCell;
        return;
    }
    q.push(std::make_pair(x, y));
    ++numOpenCell;

    //bfs
    while(q.size())
    {
        int ux = q.front().first, uy = q.front().second;
        q.pop();
        for(int dir=0; dir<SIZE; ++dir)
        {
            int vx = ux + dx[dir], vy = uy + dy[dir];
            if(!inside(vx, vy) || isHide[vx][vy] == false) //|| haveFlag[vx][vy])
                continue;
            isHide[vx][vy] = false;
            ++numOpenCell;
            if(cellType[vx][vy] == EMPTY)
                q.push(std::make_pair(vx, vy));
        }
    }
}

LGame::LGame()
{
    gWindow = NULL;
    gRenderer = NULL;
    for(int i=0; i<SIZE; ++i)
    {
        for(int j=0; j<SIZE; ++j)
        {
            cellType[i][j] = EMPTY;
            isHide[i][j] = true;
            haveBomb[i][j] = false;
            haveFlag[i][j] = false;
        }
    }
    endgame = false;
    generated = false;
    numOpenCell = 0;
    remainFlag = CNT_BOMB;
}

LGame::~LGame()
{
	//Free loaded images
	// gButtonSpriteSheetTexture.free();
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}