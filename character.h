/* 
 * File:   character.h
 * Author: nosferatu
 *
 * Created on June 26, 2015, 6:10 PM
 */

#ifndef CHARACTER_H
#define	CHARACTER_H

#include "sprite.h"
#include <ncurses.h>

enum
{
    CHAR_LEFT2 = 0,
    CHAR_LEFT1,
    CHAR_LEFT,
    CHAR_CENTER,
    CHAR_RIGHT,
    CHAR_RIGHT1,
    CHAR_RIGHT2
};

class Character : public Sprite{
    public:

        Character(DrawEngine *de, Level *lvl, int index, float x = 1, float y = 1, int numlives = 3, 
                char left_key = KEY_LEFT, char right_key = KEY_RIGHT);

        //move the pong (pure virtual Sprite method, overwritten)
        bool keyPress(char c);
        
        //check if we are hitting walls (overloaded method)
        bool isValidMove(int posx, int posy);
        
        //reset our position to the bottom center part of screen
        void resetPos();
        
        //this implementation is when the ball is not caught (overloaded method)
        void addLives(int num = 1);
        
        //draw the pong character (overloaded method)
        void draw(float x, float y);
        
        //erase the pong character (overloaded method)
        void erase(float x, float y);

        int getHitSide();
        void setHitSide(int side);
        
    protected:
        //additional data members specific for character class
        char leftKey;
        char rightKey;
        int pongArray[7];
        int hitSide;
        
};

#endif	/* CHARACTER_H */

