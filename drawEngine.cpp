#include <curses.h>

#include "drawEngine.h"
#include "sprite.h"
#include "level.h"

//constructor, init the screen size and curs vis
DrawEngine::DrawEngine(int sizeW, int sizeH) {
    
    screenWidth = sizeW; //40
    screenHeight = sizeH; //30
    
    setCursorVis(false);

}

//destructor set the curs back
DrawEngine::~DrawEngine()
{
    //set the cursor visibility back on
    setCursorVis(true);
    
    refresh();
}

//create the sprite, or assign a char for sprite drawing
int DrawEngine::createSprite(const char& c, int index)
{
    
    if(index >= 0 && index < 16)
    {
        spriteImage[index] = c;

        return index;
    }
    return -1;
    
}

//erase the curr sprite
void DrawEngine::deleteSprite(int posx, int posy)
{
    gotoxy(posy, posx);
    printw(" ");
    refresh();
}

//draw the curr sprite at specific pos
void DrawEngine::drawSprite(int index, int posx, int posy)
{
    //we have to swap x with y since ncurses reads info differently
    const char *sprite = &spriteImage[index];
    //mvprintw can print characters at specific pos
    //keep in mind posx, posy are swapped since expects
    //mvprintw(y,x,char) format
    mvprintw(posy, posx, "%c", spriteImage[index]);
    
    refresh();
}

//set the tile to a specific char
int DrawEngine::createBgTile(const char& c, int index)
{
    if(index >= 0 && index < 16)
    {
        tileImage[index] = c;
        return index;
    }
    return -1;
    
}
//go to specific pos
void DrawEngine::gotoxy(int x, int y)
{
    initscr();
    
    move(x, y);    
}

//set the cursor visibility using ncurses
void DrawEngine::setCursorVis(bool vis)
{
    if(vis)
    {
        curs_set(1);
        return;
    }
    curs_set(0);
}

//get the screen width
float DrawEngine::getScreenW()
{
    return screenWidth;
}

//get the screen height
float DrawEngine::getScreenH()
{
    return screenHeight;
}

//set the map to point to level construct
void DrawEngine::setMap(char **data)
{
    map = data;
}

//draw our block tiles
void DrawEngine::drawTile(int index, int posx, int posy)
{
    char t = tileImage[index];
    mvprintw(posy, posx, "%c" ,tileImage[index]);
    refresh();
}

//delete our block tiles
void DrawEngine::deleteTile(int posx, int posy)
{
    deleteSprite(posx, posy);
    map[posx][posy] = TILE_EMPTY;
}

//draw our tiles method
void DrawEngine::draw()
{
    if(map)
    {
        for(int w = 0; w < screenWidth; w++)
        {    
            for(int h = 0; h < screenHeight; h++)
            {
                //for some reason the drawing of our tiles is offset by 1
                int index = map[w][h];
                drawTile(index, w, h + 1);
            }
        }
    }
}

//method for drawing messages on the screen
void DrawEngine::displayMessage(const char* msg, int x, int y)
{
    //go to the specified pos
    gotoxy(x,y);
    //while the char ptr is not the last char
    while(*msg != '\0'){
        //print the message
        printw("%c", *msg);
        *msg++;
    }
    //ncurses refresh the screen
    refresh();
}
//set the tile to be empty at specific location
void DrawEngine::emptyTile(int posx, int posy)
{
    map[posx][posy] = TILE_EMPTY;
}

