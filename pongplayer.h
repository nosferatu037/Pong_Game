/* 
 * File:   pongplayer.h
 * Author: nosferatu
 *
 * Created on June 27, 2015, 8:23 PM
 */

#ifndef PONGPLAYER_H
#define	PONGPLAYER_H
#include "character.h"
#include "ball.h"

class Pongplayer : public Character
{
    public:
        Pongplayer(DrawEngine *de, Level *lvl, int index, float x = 1, float y = 1, int numlives = 3,
                char left_key = KEY_LEFT, char right_key = KEY_RIGHT, char spell = ' ');
        
        bool keyPress(char c);
        
        void setBall(Ball *b);

        
    protected:
        void castSpell();
            
    private:
        char spellKey;
        Ball *ball;

};


#endif	/* PONGPLAYER_H */

