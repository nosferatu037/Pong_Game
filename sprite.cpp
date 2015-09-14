#include "sprite.h"

Sprite::Sprite(DrawEngine* de, Level *lvl, int index, float x, float y, int numlives)
{
    drawArea = de;
    
    level = lvl;
    
    spriteIndex = index;
    
    pos.x = x;
    pos.y = y;
    
    lives = numlives;
    
    classID = SPRITE_CLASSID;
}

//executing the destructor will erase the sprite
Sprite::~Sprite()
{
    erase(pos.x, pos.y);
}

//return the pos vector of the sprite
vector Sprite::getPos()
{
    return pos;
}

//return x pos component
float Sprite::getX()
{
    return pos.x;
}

//return y pos component
float Sprite::getY()
{
    return pos.y;
}

//add/remove lives to the sprite
void Sprite::addLives(int num)
{
    lives += num;
}

//return lives count of sprite
int Sprite::getLives()
{
    return lives;
}

//return true if sprite is alive, false if dead
bool Sprite::isAlive()
{
    return (lives > 0);
}

//move the sprite
bool Sprite::move(float x, float y)
{
    vector nextpos;
    nextpos.x = (int)(pos.x + x);
    nextpos.y = (int)(pos.y + y);
    
    //if we can move to our next/future position
    if(isValidMove(nextpos.x, nextpos.y))
    {
        //erase the current sprite
        erase(pos.x, pos.y);
        
        pos.x += x;
        pos.y += y;
        
        //update the facing dir so we know which way we are facing
        //im not sure this is needed for the pong stuff but lets see
        facingDir.x = x;
        facingDir.y = y;
        
        draw(pos.x, pos.y);
        return true;
    }
    return false;
}

//check if the move is valid, if we are not hitting walls
bool Sprite::isValidMove(int posx, int posy)
{
    if(posx >=0 && posy >= 0)
    {
        if(posx < drawArea->getScreenW() && posy < drawArea->getScreenH())
        {
            return true;
        }
    }
    return false;
}

//draw the sprite
void Sprite::draw(float x, float y)
{
    drawArea->drawSprite(spriteIndex, (int)x, (int)y);
}

//delete the sprite
void Sprite::erase(float x, float y)
{
    drawArea->deleteSprite((int)x, (int)y);
}
