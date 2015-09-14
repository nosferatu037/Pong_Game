#include "blocks.h"

Blocks::Blocks(DrawEngine* de, Level* lvl, int index, float x, float y, int numlives, int id, bool block)
                : Sprite(de, lvl, index, x, y, numlives)
{
    blockid = id;
    isBlock = block;
    classID = BLOCK_CLASSID;
}
//empty method since its pure virtual inherited from sprite
//we dont need to implement this
bool Blocks::keyPress(char c)
{
    
}
//set blocks facing direction
bool Blocks::setFacingDir(float dirx, float diry)
{
    facingDir.x = dirx;
    facingDir.y = diry;
}
//get blocks facing dir
float Blocks::getFacingDirX()
{
    return facingDir.x;
}

float Blocks::getFacingDirY()
{
    return facingDir.y;
}


int Blocks::getID()
{
    return blockid;
}

void Blocks::setID(int id)
{
    blockid = id;
}