#include "game.h"
#include <iostream>
#include "kbhit.h"
#include <curses.h>
#include <curses.h>
#include "timeGetTime.h"
#include "level.h"
#include "pongplayer.h"
#include <cmath>

using namespace std;

#define GAME_SPEED 100


void Game::run()
{
    //ncurses init stuff
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    
    //timer and fps stuff
    startTime = timeGetTime();
    endTime = 0;

    //get the tty size
    float max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    //set the game window size
    de = new DrawEngine(max_x,max_y);
    
    //create new level
    lvl = new Level(de);
    
    //set the pong draw type
    de->createSprite('=', SPRITE_PLAYER);
    de->createSprite('o', SPRITE_BALL);
    de->createBgTile(' ', TILE_EMPTY);
    de->createBgTile('+', TILE_BLOCK);
    
    //create a new player/pong at bottom screen center
    player = new Pongplayer(de, lvl, 0, max_x / 2, max_y - 2);
    ball = new Ball(de, lvl, 1, max_x / 2, max_y - 3);

    //connect the ball to the player 
    player->setBall(ball);
    //connect the player to the ball
    ball->setPlayer(player);
    
    //add him to the level
    lvl->addPlayer(player);
    //add the ball to the level
    lvl->addBall(ball);
    
    //draw the level
    lvl->draw();
    
    //draw the player/pong at the same pos
    player->draw(max_x / 2, max_y - 2);
    ball->draw(max_x / 2, max_y - 3);
    
    int key = ' ';
    //if we dont press q 
    while(key != 'q')
    {
        //and if the player is alive
        if(player->isAlive())
        {
            //if the player is alive and we are not hitting any keys
             while(!getInput(&key) && player->isAlive())
            {
                //update our ball movement
                timerUpdate();
                
            }
            
            //if we have pressed a key move the player
            lvl->keyPress(key);
        }
        //if the player is dead
        else
        {
            //tell the user player is dead and get out of the loop
            de->displayMessage("You have lost all lives, Game Over!", max_x / 2, max_y / 2);
            break;
        }
        
    }
    //refresh the screen and end it
    refresh();
    endwin();
    return;
                        
}

//calculate the ball move
void Game::timerUpdate()
{
    //set the current time
    double currTime = abs(timeGetTime() - endTime);
    
    //if current time is less than our game speed
    //exit timerUpdate method
    if(currTime < GAME_SPEED)
        return;
    
    //if not update our level
    lvl->update();
    
    //fetch the end time in this method
    endTime = timeGetTime();
}

//figure out if a key has been pressed
bool Game::getInput(int* c)
{
    if(kbhit())
    {
        *c = getch();
        if(*c != ERR){
            return true;
        }
        napms(5);
    }
    return false;
}
