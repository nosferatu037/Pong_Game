/* 
 * File:   blocks.h
 * Author: nosferatu
 *
 * Created on June 28, 2015, 5:35 PM
 */

#ifndef BLOCKS_H
#define	BLOCKS_H
#include "sprite.h"


class Blocks : public Sprite
{
    public:
        Blocks(DrawEngine *de, Level *lvl, int index, float x, float y, int numlives, int id, bool block);
        //we need to overload this since its a pure virtual method
        bool keyPress(char c);
        //data member for signaling if block is empty or not
        bool isBlock;
        //set blocks facing dir
        bool setFacingDir(float dirx, float diry);
        //get blocks facing dir
        float getFacingDirX();
        float getFacingDirY();


        int getID();
        void setID(int id);
    protected:
       
        int blockid;
};


#endif	/* BLOCKS_H */

