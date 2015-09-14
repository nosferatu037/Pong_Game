/* 
 * File:   sprite.h
 * Author: nosferatu
 *
 * Created on June 26, 2015, 5:13 PM
 */

#ifndef SPRITE_H
#define	SPRITE_H

#include "drawEngine.h"
#include "level.h"

enum 
{
    SPRITE_CLASSID,
    CHAR_CLASSID,
    PONGPLAYER_CLASSID,
    BALL_CLASSID,
    BLOCK_CLASSID
};

struct vector{

    float x;
    float y;
};

class Sprite
{
public:
    Sprite(DrawEngine *de, Level *lvl, int index, float x, float y, int numlives = 1);
    ~Sprite();
    
    //get the position information method
    vector getPos();
    float getX();
    float getY();
    
    //return the lives number method
    int getLives();
    
    //this method will be overwritten by other class children
    virtual void addLives(int num = 1);
    
    //check if we are alive method
    bool isAlive();
    
    //check if we can move to specific pos
    //this method will be overwritten by subclasses
    virtual bool isValidMove(int posx, int posy);
    
    //virtual non implemented function
    //we NEED to implement this pure virtual method 
    //in any class that inherits Sprite class
    virtual bool keyPress(char c) = 0;

    //    virtual bool idleUpdate();
    //move to specific position method
    //this can be overwritten by subclasses
    virtual bool move(float x, float y);
    
    //store our classID
    int classID;
    
    //blocks and player will have to be able to have a custom draw erase method
    virtual void draw(float x, float y);
    virtual void erase(float x, float y);
    
protected:
    //all the data members for communication 
    Level *level;
    
    vector pos;
    
    int lives;
    
    int spriteIndex;
    
    vector facingDir;
    
    DrawEngine *drawArea;
    
    
};


#endif	/* SPRITE_H */

