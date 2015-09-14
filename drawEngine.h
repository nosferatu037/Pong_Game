/* 
 * File:   drawEngine.h
 * Author: nosferatu
 *
 * Created on June 26, 2015, 4:19 PM
 */

#ifndef DRAWENGINE_H
#define	DRAWENGINE_H


class DrawEngine
{
public:
    DrawEngine(int sizeW = 40, int sizeH = 30);
    ~DrawEngine();
    
    //create and manipulate drawing of the Sprites
    int createSprite(const char& c, int index);
    void drawSprite(int index, int posx, int posy);
    void deleteSprite(int posx, int posy);
    
    
    //create and manipulate drawing of the level tiles
    int createBgTile(const char& c, int index);
    void drawTile(int index, int posx, int posy);
    void deleteTile(int posx, int posy);
    //set the tile to be empty at specific location
    void emptyTile(int posx, int posy);
    
    //get screen size
    float getScreenW();
    float getScreenH();
    //point to the level map pointer
    void setMap(char **data);
    //draw our tiles method 
    void draw();
    
    //display a message
    void displayMessage(const char* msg, int x, int y);
    
private:
    //go to specific location on the screen method
    void gotoxy(int x, int y);
    //set our cursor visibility
    void setCursorVis(bool vis);
    
    
protected:
    //data members for holding draw related info
    char spriteImage[16];
    char tileImage[16];
    char **map;
    int screenWidth, screenHeight;
};

#endif	/* DRAWENGINE_H */

