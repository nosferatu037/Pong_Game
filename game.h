/* 
 * File:   game.h
 * Author: nosferatu
 *
 * Created on June 26, 2015, 3:34 PM
 */

#ifndef GAME_H
#define	GAME_H
#include "drawEngine.h"
#include "character.h"
#include "ball.h"
#include "level.h"
#include "pongplayer.h"

class Game{
public:

    //main method for running the game
    void run();
    //fetch the input from the user method
    bool getInput(int *c);
    //method to update our timer for controlling game speed
    void timerUpdate();
    
private:
    //store our time data members
    int startTime;
    int endTime;
    
    //ptrs to the draw engine, player, ball and level
    DrawEngine *de;
    Pongplayer *player;
    Ball *ball;
    Level *lvl;
    
};


#endif	/* GAME_H */

