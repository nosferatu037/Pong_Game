#include "character.h"

Character::Character(DrawEngine* de, Level *lvl, int index, float x, float y, int numlives, 
                        char left_key, char right_key) 
                        : Sprite(de, lvl, index, x, y, numlives)
{
    hitSide = -1;
    //set our move keys
    leftKey = left_key;
    rightKey = right_key;
    //set our classID
    classID = CHAR_CLASSID;
    //set the size of the pong player
    for(int i = CHAR_LEFT2; i <= CHAR_RIGHT2; i++){
        pongArray[i] = i;
    }
    
}
//move the pong
//overwritten pure virtual method from Sprite class
bool Character::keyPress(char c)
{
    if(c == leftKey)
    {
        return move(-1, 0);
    }
    else if(c == rightKey)
    {
        return move(1,0);
    }
    else
    {
        return false;
    }
}

//this implementation is when the ball is not caught
void Character::addLives(int num)
{
    //remove lives
    //for now the pong can only lose lives
    Sprite::addLives(num);
    //if we are still alive
    if(Sprite::isAlive())
    {
        //reset pongs pos
        resetPos();
    }
}

//draw the pong character
void Character::draw(float x, float y)
{
    //for the entire length of pong draw the pong
    for(int i = CHAR_LEFT2; i <= CHAR_RIGHT2; i++){
        drawArea->drawSprite(spriteIndex, (int)(x - (3 - i)),(int) y); 
//    drawArea->drawSprite(spriteIndex, x, y);
    }
}

//erase the pong character
void Character::erase(float x, float y)
{
    //for the entire length of pong erase the pong
    for(int i = CHAR_LEFT2; i <= CHAR_RIGHT2; i++){
        drawArea->deleteSprite(int(x - (3 - i)),(int) y); 
    }
//    drawArea->deleteSprite(x, y);
}

//check if we are hitting walls
bool Character::isValidMove(int posx, int posy)
{
    //we need to consider the offset for the pongs length
    if((posx - 3) >=0 && posy >= 0)
    {
        if((posx + 3) < drawArea->getScreenW())
        {
            return true;
        }
    }
    return false;
}

//reset our position to the bottom center part of screen
void Character::resetPos()
{
    //erase current pos
    erase(pos.x, pos.y);
    //set the pos to bottom center of screen
    pos.x = drawArea->getScreenW() / 2;
    pos.y = drawArea->getScreenH() - 2;
    //draw our new pos
    draw(drawArea->getScreenW() / 2, drawArea->getScreenH() - 2);
}

int Character::getHitSide()
{
    return hitSide;
}

void Character::setHitSide(int side)
{
    hitSide = side;
}

