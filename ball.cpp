#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ball.h"
#include "character.h"


Ball::Ball(DrawEngine* de, Level* lvl, int index, float x, float y, 
        int numlives, char left_key, char right_key) : 
        Sprite(de, lvl, index, x, y, numlives)
{
    //set our random direction
    srand(time(NULL));
    facingDir.y = -(0.5 * ((double) rand() / (RAND_MAX)) + 0.5);
    facingDir.x = (1 - 2 * ((rand() % 100)/100.0));
//    facingDir.x = ((double) rand() / (RAND_MAX)) * 0.5;
//    facingDir.y = -2 * ((rand() % 100)/100.0);
    
    
    facingDir = normalize(facingDir);
    
    //init ballOn to false, make the ball sit on pong
    ballOn = false;
    
    leftKey = left_key;
    rightKey = right_key;
    
    moveCounter = 0;
    classID = BALL_CLASSID;
    hitSide = HIT_NOHIT;
    lives = 3;
}
//set the pointer to player
void Ball::setPlayer(Character* p)
{
    player = p;
}

//update the balls position
void Ball::update()
{
    if(ballOn)
    {
        //signal that we have started to move from the pong
        if(moveCounter < 2)
        {
            moveCounter++;
        }
        //get the future/new position
        vector newpos;
        facingDir = normalize(facingDir);
        
        newpos.x = (int)(pos.x + facingDir.x);
        newpos.y = (int)(pos.y + facingDir.y);
        
        //check for collision
        int collision = checkMove(newpos.x, newpos.y);
        
        //are we are not hitting anything
        if(collision == 2)
        {
            //if we are not hitting the bricks
            if(level->getLevelInfo(newpos.x, newpos.y) == TILE_EMPTY)
            {
                //keep moving
                move(facingDir.x, facingDir.y);
            }
            //if we are hitting the BLOCKS
            else
            {
                //move the ball on top of the block
                move(facingDir.x,facingDir.y);
                
                //get the current blocks normal
                vector normDir;
                normDir = level->getEnemiesNormal(newpos.x, newpos.y);
                
                //kill the block
//                drawArea->emptyTile(newpos.x, newpos.y);
                level->emptyBlock(newpos.x, newpos.y);
                //update our blocks normals since we have killed/deleted a block
//                level->updateEnemiesDir();
                //update our blocks and empty spaces
                level->updateEnemies();
                
                drawArea->drawSprite(SPRITE_BALL, newpos.x, newpos.y);
//                //get the current blocks normal true - using laplace, false - no laplace
//                vector normDir;
//                normDir.x = level->getEnemiesDirX(newpos.x, newpos.y);
//                normDir.y = level->getEnemiesDirY(newpos.x, newpos.y);
                //bounce the ball off of the blocks normal and set
                //the last member to true meaning that we are conducting bounce off blocks
                bounceBall(normDir.x, normDir.y, true);
            }
        }
        //if we are hitting the pong player
        else if(collision == 1)
        {
            //bounce the ball based on the side of the player we hit
            bounceBall(0,0,false);
        }
        //if we are hitting the wall
        else if(collision == 0)
        {
            //just bounce the ball
            bounceBall(0,0,false);
        }
        //if we have died
        else if(collision == -1)
        {
            //if player is alive
            if(player->isAlive())
            {
                //remove balls lives/players lives
                player->addLives(-1);
                addLives(-1);

                //reset the ball position and the players pos
                resetPos();
                player->resetPos();
            }
        }
    }
}

//check if we can move 
//return 2 if the space is empty, we havent died and hit the pong player
//return 1 if we have hit the pong player
//return 0 if we have hit a wall
//return -1 if we went down too far and have died
int Ball::checkMove(int posx, int posy)
{
    
    //check if the space is empty, by checking
    //if we are not hitting the wall or have died
    bool wall = isNotHittingWall(posx, posy);
    
    //we havent hit the wall
    if(wall)
    {
        //we can go and check 
        //if we are not hitting the pong player
        bool p = isNotHittingPlayer(posx, posy);
        
        //if we havent hit the player
        if(p)
        {
            //did we die?
            bool die = notDead(posx, posy);
            //if we didnt die
            if(die)
            {
                //signal that we can move (return 2)
                return 2;
            }
            //if we have died
            else
            {
                //signal that we have died (return -1)
                return -1;
            }
        }
        //if we have hit the player
        else
        {
            //signal that we have hit the player (return 1)
            return 1;
        }
    }
    //if we have hit the wall
    else
    {
        //signal that we have hit the wall (return 0)
        return 0;
    }
    
}

//are we dead?
bool Ball::notDead(int posx, int posy)
{
    if((int)posy < (player->getY()))
    {
        return true;
    }
    return false;
}

//are we hitting the player?
bool Ball::isNotHittingPlayer(int posx, int posy)
{
    //if we still havent moved from pongs pos
    //or we just moved a moment ago
    if(moveCounter > 1)
    {
        //if we are not dead, gone too low
        //or if we are not just above the pong player
        bool distOn = false;
        float distance;
        
        //aimPlayer is going to point towards the ball from player
        //or maybe its better to just use the normal of the player
        vector aimPlayer;
        aimPlayer.x = 0;
        aimPlayer.y = -1;
        //normalize our aim vector
        aimPlayer = normalize(aimPlayer);

        //get the dot product between the balls facingDir
        //and the vector from player to ball
        float dotProd = dotProduct(facingDir, aimPlayer);
        
        //get the distance from ball to player
        //we also need to sum up the distance from all the parts 
        //of the pong player and not only from the center
        int i, tmpi;
        float minDist = 100;
        for(i = -3; i <= 3; i++)
        {
            distance = sqrt(((player->getX() + i) - posx) * ((player->getX() + i) - posx) 
                        + (player->getY() - posy) * (player->getY() - posy));
                        
            if(distance < minDist)
            {
                minDist = distance;
                tmpi = i;
            }
            else
            {
                break;
            }
 
        }
     
        if(minDist < 1.5)
        {
            distOn = true;
        }
        //if dotProd < 0 - if the facingDir is facing towards the player
        //and if distance is pretty small
        //we are probably pretty low heading towards the bottom
        if(dotProd < 0 && distOn)
        {
            //if we are not dead
            if(notDead(posx, posy))
            {
                //if we are just above the player
                if((int)posy > (player->getY() - 2))
                {
                    //if we are not in pongs player width vicinity 
                    if(((int)posx > (3 + player->getX()) && (int)posx < (player->getX() - 3)))
                    {
                        //we havent hit the player
                        hitSide = HIT_NOHIT;
                        return true;
                    }
                    player->setHitSide(tmpi);
                    //we have hit the player
                    hitSide = HIT_PLAYER;
                    return false;
                }
            }
            //we are either dead or have hit the player
            hitSide = HIT_NOHIT;
            return false;
        }
        hitSide = HIT_NOHIT;
        return true;
    }
    //we are not moving and havent hit anything
    hitSide = HIT_NOHIT;
    return true;
}

//are we hitting the wall?
bool Ball::isNotHittingWall(int posx, int posy)
{
    //are we hitting left side
    if((int)posx == 0)
    {
        hitSide = HIT_LSIDE;
        return false;
    }
    //or are we hitting right side
    if((int)posx == drawArea->getScreenW())
    {
        hitSide = HIT_RSIDE;
        return false;
    }
    //or are we hitting top
    if((int)posy == 0)
    {
        hitSide = HIT_TOP;
        return false;
    }
    //or are we hitting bottom
    if((int)posy == drawArea->getScreenH())
    {
        hitSide = HIT_BOTTOM;
        return false;
    }
    //if not then we are not hitting the wall
    else
    {
        hitSide = HIT_NOHIT;
        return true;
    }
}

//add/remove lives from the ball
void Ball::addLives(int num)
{
    Sprite::addLives(num);
}

//bounce the ball
//the most important part of the game
bool Ball::bounceBall(float x, float y, bool block)
{
    //if we are hitting an enemy block
    if(block)
    {
        //create temp vectors 
        vector collNorm, tempDir;
        
        //set the enemies collision normal
        collNorm.x = x;
        collNorm.y = y;
        //calc the reflection vector from our dir and 
        //enemies collision normal
        tempDir = reflect(facingDir, collNorm);
        
        //just check to see if our y is extremely low, cannot have that
        if(fabs(tempDir.y) < 0.1)
            tempDir.y += 0.15;
        tempDir = normalize(tempDir);
        
        //set our facing dir
        facingDir = tempDir;
        //we are successful
        return true;
    }
    //if we are not hitting an enemy block
    //check what is it that we are hitting
    else
    {
        //are we hitting bottom wall
        if(hitSide == HIT_BOTTOM)
        {
            //first move to bottom side of the wall
            if((int)pos.y != drawArea->getScreenH())
            {
//                moveToPos((pos.x + facingDir.x), drawArea->getScreenH());
//                move(facingDir.x, drawArea->getScreenH() - pos.y);
                move(facingDir.x, facingDir.y);
            }
            
            //then update our facing dir
            //so next run we will bounce off of the wall
            vector bottom, tempDir;
            bottom.x = 0;
            bottom.y = -1;
            
            tempDir = reflect(facingDir, bottom);
            //just check to see if our y is extremely low, cannot have that
            if(fabs(tempDir.y) < 0.1)
                tempDir.y += 0.15;
            tempDir = normalize(tempDir);
            
            facingDir = tempDir;
            
            return true;
        }
        //or top 
        else if(hitSide == HIT_TOP)
        {
            //first move to top side of the wall
            if((int)pos.y != 0)
            {
//                moveToPos((pos.x + facingDir.x), 0);
//                move(facingDir.x, 0 - pos.y);
                move(facingDir.x, facingDir.y);
            }
            
            //then update our facing dir
            //so next run we will bounce off of the wall
            vector top, tempDir;
            top.x = 0;
            top.y = 1;
            
            tempDir = reflect(facingDir, top);
            //just check to see if our y is extremely low, cannot have that
            if(fabs(tempDir.y) < 0.1)
                tempDir.y += 0.15;
            tempDir = normalize(tempDir);
            facingDir = tempDir;
            
            return true;
        }
        //or left side
        else if(hitSide == HIT_LSIDE)
        {
            //first move to left side of the wall
            if((int)pos.x != 0)
            {
//                moveToPos(0,(pos.y + facingDir.y));
//                move(0 - pos.x, facingDir.y);
                move(facingDir.x, facingDir.y);
            }
            
            //then update our facing dir
            //so next run we will bounce off of the wall
            vector lside, tempDir;
            lside.x = 1;
            lside.y = 0;
            
            tempDir = reflect(facingDir, lside);
            //just check to see if our y is extremely low, cannot have that
            if(fabs(tempDir.y) < 0.1)
                tempDir.y += 0.15;
            tempDir = normalize(tempDir);
            facingDir = tempDir;
            
            return true;
        }
        //or right side
        else if(hitSide == HIT_RSIDE)
        {
            //first move to right side of the wall
            if((int)pos.x != drawArea->getScreenW())
            {
//                moveToPos(drawArea->getScreenW(), (pos.y + facingDir.y));
//                move(drawArea->getScreenW() - pos.x, facingDir.y);
                move(facingDir.x, facingDir.y);
            }
            
            //then update our facing dir
            //so next run we will bounce off of the wall
            vector rside, tempDir;
            rside.x = -1;
            rside.y = 0;
            
            tempDir = reflect(facingDir, rside);
            //just check to see if our y is extremely low, cannot have that
            if(fabs(tempDir.y) < 0.1)
                tempDir.y += 0.15;
            tempDir = normalize(tempDir);
            facingDir = tempDir;
            
            return true;
        }
        else if(hitSide == HIT_PLAYER)
        {
            //first move to bottom side of the wall
            if((int)pos.y != drawArea->getScreenH())
            {
                move(facingDir.x, facingDir.y);
            }
            
            vector normal, tempDir;
            int side = player->getHitSide();

            normal.x = (side) * (0.7 / 3.0);
            normal.y = -1;
            
            normal = normalize(normal);

            tempDir = reflect(facingDir, normal);
            //just check to see if our y is extremely low, cannot have that
            if(fabs(tempDir.y) < 0.1)
                tempDir.y += 0.15;
            tempDir = normalize(tempDir);
            facingDir = tempDir;
            
            return true;
            
        }
        //or maybe nothing
        else
        {
            return false;
        }
        
        
    }
}


//draw the ball
void Ball::draw(float x, float y)
{
    drawArea->drawSprite(SPRITE_BALL, x, y);
}

//erase the ball
void Ball::erase(float x, float y)
{
    drawArea->deleteSprite(x, y);
}

//move the ball if it hasnt been launched yet
bool Ball::keyPress(char c)
{
    if(c == leftKey)
    {
        return move(-1, 0);
    }
    else if(c == rightKey)
    {
        return move(1, 0);
    }
    else
    {
        return false;
    }
}

//set the ball status
//ballOn = true if the ball has been launched
//ballOn = false if its still sitting on the player
void Ball::setBallOn(bool set)
{
    ballOn = set;
}

//return the ball status
bool Ball::getBallStatus()
{
    return ballOn;
}

//move our ball based off of facingDir
//this is a relative move
bool Ball::move(float x, float y)
{
    //this is a relative move
    //meaning we move by increment (accel/facingDir)
    //rather than moving to actual pos
    erase(pos.x, pos.y);

    //just add to our current pos
    pos.x += x;
    pos.y += y;

    draw(pos.x, pos.y);

    return true;
}

//move the ball at specific location
//this is an absolute move
bool Ball::moveToPos(float x, float y)
{
    //this is an absolute move
    //meaning we draw the ball at a specific location
    //without updating the facingDir
    //first erase the ball at current pos
    erase(pos.x, pos.y);
    
    //then update our future pos
    pos.x = x;
    pos.y = y;
    
    //draw us at the future pos 
    draw(pos.x, pos.y);
    
    return true;
}

//reset the balls position and the center bottom screen 
//on top of the pong player, also reset the facingDirection
void Ball::resetPos()
{
    //erase ball from the dead pos
    erase(pos.x, pos.y);
    //reset the pos
    pos.x = drawArea->getScreenW() / 2;
    pos.y = drawArea->getScreenH() - 3;
    //draw at the default pos
    draw(drawArea->getScreenW() / 2, drawArea->getScreenH() - 3);
    //regenerate the facingDir
    srand(time(NULL));
    facingDir.x = (1 - 2 * ((rand() % 100)/100.0));
    facingDir.y = -2 * ((rand() % 100)/100.0);
    
    facingDir = normalize(facingDir);
    
    //reset the pos on top of the pong
    ballOn = false;   
    //and our move counter
    moveCounter = 0;
    //and our hitSide
    hitSide = HIT_NOHIT;
}

//return a dot product between two vectors
float Ball::dotProduct(vector dir, vector norm)
{
//    float dirMag = sqrt(dir.x * dir.x + dir.y * dir.y);
//    float normMag = sqrt(norm.x * norm.x + norm.y * norm.y);
//    float costh = cos(dirMag/normMag);
    
//    return (dirMag * normMag * costh);
    return (dir.x * norm.x + dir.y * norm.y);
}

//calculate the reflection vector when we collide
//between our direction and enemies normal
vector Ball::reflect(vector dir, vector norm)
{
    vector temp;
    float dot = dotProduct(dir, norm);
    temp.x = dir.x - 2 * dot * norm.x;
    temp.y = dir.y - 2 * dot * norm.y;
    return temp;
}

//calc vectors magnitude
float Ball::vecMag(vector Vec)
{
    return (sqrt(Vec.x * Vec.x + Vec.y * Vec.y));
}

//normalize a vector
vector Ball::normalize(vector Vec)
{
    vector tmp;
    tmp.x = Vec.x / vecMag(Vec);
    tmp.y = Vec.y / vecMag(Vec);
    
    return tmp;
}

//get the facing direction
vector Ball::getFacingDir()
{
    return facingDir;
}
