#include <stdlib.h>
#include <iterator>
#include "level.h"
#include "character.h"
#include "ball.h"
#include "blocks.h"
#include "fitrange.h"

#define BLOCK_SIZE      3
#define BLOCK_WIDTH     22
#define BLOCK_HEIGHT    10
#define BLOCK_HOFFSET   2

Level::Level(DrawEngine* de)
{
    drawArea = de;
    
    width = de->getScreenW();
    height = de->getScreenH();
    
    //create the level walls 
    //AND the blocks pong will destroy
    level = new char*[width];
        
    for(int w = 0; w < width; w++)
    {
        level[w] = new char[height];
    }
        
    player = NULL;
    ball = NULL;
    
    //create our level
    createLevel();
    //update our enemies direction
    //calculate the normals from laplacian
    updateEnemiesDir();
    //set the map to point to level construct
    drawArea->setMap(level);
    
}

//clear all the level maps and npcs
Level::~Level()
{
    for(int w = 0; w < width; w++)
    {
        delete [] level[w];
    }
    delete [] level;
    
    for(iter = npc.begin(); iter != npc.end(); ++iter)
    {
        delete *iter;
    }
}

//add a player to the level
void Level::addPlayer(Character* p)
{
    player = p;
}

//add the ball to the level
void Level::addBall(Ball* b)
{
    ball = b;
}

//create our level!!
void Level::createLevel()
{
    //since we want to be able to make each block consist of many chars
    //we setup an id matrix system where we determine the ids based off of BLOCK_SIZE
    //  EXAMPLE
    //BLOCK_SIZE = 1    //BLOCK_SIZE = 3
    //-------------     -------------------
    //| 1 | 4 | 7 |     | 1 | 1 | 1 | 4 | 4...
    //-------------     -------------------
    //| 2 | 5 | 8 |     | 2 | 2 | 2 | 5 | 5...
    //-------------     -------------------                    
    //| 3 | 6 | 9 |     | 3 | 3 | 3 | 6 | 6...
    //----------------------------------------
    int countw ,counth, lastCount, prevLastCount;
    countw = counth = lastCount = prevLastCount = 0;

    //iterate thru our full screen size
    for(int w = 0; w < width; w++)
    {
        //if we are at the beginning of our iteration
        //basically if we are at beginning of every 0th iteration of BLOCK_SIZE size
        //example BLOCK_SIZE = 1, iterations: 0-0-0-0-0-0-0-0-...
        //example BLOCK_SIZE = 2, iterations: 0-1-0-1-0-1-0-1-...
        //example BLOCK_SIZE = 3, iterations: 0-1-2-0-1-2-0-1-...
        if((w % ((width / 2) - BLOCK_WIDTH / 2)) % BLOCK_SIZE == 0)
        {
            //reset the counter to the last recorded one
            counth = lastCount;
        }
        //if we are every nth iteration, so not the 0th
        else
        {
            //reset the counter to the previous last count
            counth = prevLastCount;
        }
        
        for(int h = 0; h < height; h++)
        {
            //if we are within a certain limit of the width
            if((w >= ((width / 2) - BLOCK_WIDTH / 2) && w < ((width / 2) + BLOCK_WIDTH / 2)))
            {
                //this is a counter for every time we get successfully into our loop 
                countw = w  % ((width / 2) - BLOCK_WIDTH / 2);
                
                //if we within a limit of the height
                if(h >= ((height / 2) - ((BLOCK_HEIGHT / 2) + BLOCK_HOFFSET)) 
                        && h < (((height / 2) + (BLOCK_HEIGHT / 2)) - BLOCK_HOFFSET))
                {
                    //record the previous state of the lastCount
                    //every time we get to the last member of our array
                    //in the last iteration before reseting the iter to 0, 
                    //record the state of the lastCount
                    if((countw % BLOCK_SIZE == BLOCK_SIZE - 1) && 
                            (h == (((height / 2) + (BLOCK_HEIGHT / 2)) - BLOCK_HOFFSET) - 1))
                    {
                        prevLastCount = lastCount;
                    }
                    
                    //keep increasing our iterator
                    counth++;
                    //keep setting the last count to the current iter
                    //if we are at the first stage of iteration (0th iteration)
                    if(countw % BLOCK_SIZE == 0)
                    {
                        lastCount = counth;
                    }

                    //set the value to block
                    level[w][h] = TILE_BLOCK;
                    //create a new block object inside a list and set its isBlock = true
                    npc.push_back(new Blocks(drawArea, this, SPRITE_CLASSID, w, h, 1, counth, true));
                }
                else
                {
                    //set an empty block
                    level[w][h] = TILE_EMPTY;
                    //create a new block object inside a list and set its isBlock = false
                    npc.push_back(new Blocks(drawArea, this, SPRITE_CLASSID, w, h, 0, -1, false));
                }         
            }
            else
            {
                //set an empty block
                 level[w][h] = TILE_EMPTY;
                //create a new block object inside a list and set its isBlock = false
                npc.push_back(new Blocks(drawArea, this, SPRITE_CLASSID, w, h, 0, -1, false));
            }
        }
    }
}

//draw our level
void Level::draw()
{
    drawArea->draw();
}

//check if we have pressed a key in the level
bool Level::keyPress(char c)
{
    //if we have a player
    if(player)
    {
        //if the key is pressed
        //and is one of the keys mapped to the player
        if(player->keyPress(c) || c == ' ')
        {
            //if the ball hasnt moved yet
            if(ball->getBallStatus() == false)
            {
                //move the ball with the player
                if(ball->keyPress(c))
                {
                    return true;
                }
                return false;
            }
            return true;
        }
        return false;
    }
    
}

//update our ball trajectory 
void Level::update()
{
    //if the ball has left the pong
    if(ball->getBallStatus())
    {
        //update our balls info
        ball->update();
    }
}

//return the level info
//true for TILE_BLOCK
//false for TILE_EMPTY
bool Level::getLevelInfo(int x, int y)
{
    return level[x][y];
}

//update our enemies facingDirection
//basically its a normal pointing from the enemy to outter space
void Level::updateEnemiesDir()
{
    //update the enemies status
    updateEnemies();
    
    int count = 0;
    //for the entire level 
    for(int w = 0; w < width; w++)
    {
        for(int h = 0; h < height; h++)
        {   
            //if a block is not empty
            if(level[w][h] == TILE_BLOCK)
            {
                //update its normal
                updateDirectionLaplace(count);
                
            }
            count++;
        }
    }
    
//    avgFacingDir();
    
}
//======================================================================
//LAPLACIAN normal direction of enemies
//update our normal direction of the enemy
//by sampling the neighbourhood of 8 cells around the current enemy
//|--------|--------|--------|
//| pt+w-1 | pt + w | pt+w+1 |
//|--------|--------|--------|
//| pt - 1 |   pt   | pt + 1 |
//|--------|--------|--------|
//| pt-w-1 | pt - w | pt-w+1 |
//|--------|--------|--------|
//
//      pt is the current enemy 
//if the cell is empty then add it to accum, since we want our normal
//to point to the mean pos of the empty cells
//its like a laplacian in houdini vex
//==================================
void Level::updateDirectionLaplace(int index)
{
    int currIndex = index;
    //set the iterator to first block
    iter = npc.begin();
    
    //recalc the pos
    //get the width in the 2d array
    int scW = index / height;
    //get the height in the 2d array
    int scH = index % height;
    
    //===========================================
    //**********GET THE SURROUNDING BLOCKS VALUE
    bool neigh[8];
    int emptyCount = 0;
    int accumH = 0;
    int accumW = 0;
    
    //get the top left block (scW - 1,scH - 1)
    //formula for converting 2d array to lists index
    //list index = (currentWidth +/- offsetW) * fullHeight + (currentHeight +/- offsetH)
    index = (scW - 1) * height + (scH - 1);
    
    //set the iterator to point at the top left enemy
    advance(iter, index);
    
    //set its isBlock(true/false) value to the array
    neigh[0] = (*iter)->isBlock;
    
    //if its not a block (isBlock = false), it must be empty
    if(neigh[0] == 0)
    {
        //accumulate the empty blocks pos cooord
        accumH += (scH - 1);
        accumW += (scW - 1);
    }
    //increase the empty counter if we have found an empty block
    emptyCount += (neigh[0] == 0);
    
    //!!!!!REPEAT THE ABOVE PROCEDURE FOR EVERY SURROUNDING CELL!!!!
    
    
    //get the bottom left block (scW - 1,scH + 1)
    index = (scW - 1) * height + (scH + 1);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[1] = (*iter)->isBlock;
    if(neigh[1] == 0)
    {
        accumH += (scH + 1);
        accumW += (scW - 1);
    }
    emptyCount += (neigh[1] == 0);
    
    //get the top right block (scW + 1,scH - 1)
    index = (scW + 1) * height + (scH - 1);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[2] = (*iter)->isBlock;
    if(neigh[2] == 0)
    {
        accumH += (scH - 1);
        accumW += (scW + 1);
    }
    emptyCount += (neigh[2] == 0);
    
    //get the bottom right block (scW + 1,scH + 1)
    index = (scW + 1) * height + (scH + 1);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[3] = (*iter)->isBlock;
    if(neigh[3] == 0)
    {
        accumH += (scH + 1);
        accumW += (scW + 1);
    }
    emptyCount += (neigh[3] == 0);
    
    //get the top block (scW ,scH - 1)
    index = (scW) * height + (scH - 1);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[4] = (*iter)->isBlock;
    if(neigh[4] == 0)
    {
        accumH += (scH - 1);
        accumW += (scW);
    }
    emptyCount += (neigh[4] == 0);
    
    //get the bottom block (scW ,scH + 1)
    index = (scW) * height + (scH + 1);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[5] = (*iter)->isBlock;
    if(neigh[5] == 0)
    {
        accumH += (scH + 1);
        accumW += (scW);
    }
    emptyCount += (neigh[5] == 0);
    
    //get the right block (scW + 1,scH)
    index = (scW + 1) * height + (scH);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[6] = (*iter)->isBlock;
    if(neigh[6] == 0)
    {
        accumH += (scH);
        accumW += (scW + 1);
    }
    emptyCount += (neigh[6] == 0);
    
    //get the left block (scW - 1,scH)
    index = (scW - 1) * height + (scH);
    iter = npc.begin();
    advance(iter, index);
    
    neigh[7] = (*iter)->isBlock;
    if(neigh[7] == 0)
    {
        accumH += (scH);
        accumW += (scW - 1);
    }
    emptyCount += (neigh[7] == 0);
    
    //===========================================
    //we have finished the accumulation
    //now lets calculate the normal
    int newH = 0;
    int newW = 0;
    
    //if there are empty blocks
    //take their summed up coords and get an average by div with the total number of empty blocks
    if(emptyCount > 0)
    {
        newH = accumH / emptyCount;
        newW = accumW / emptyCount;
    
        //have the iterator point to the current block
        iter = npc.begin();
        advance(iter, currIndex);
        //set the current blocks facingDir to be the avg normal we just calculated
        (*iter)->setFacingDir(newW - ((*iter)->getX()), newH - ((*iter)->getY()));
    }
    
    
}
//update our enemies info
//maybe some blocks have been destroyed, mark them as empty 
//so we dont collide again
void Level::updateEnemies()
{
    int count = 0;
    //set the iterator to the beginnig of npc
    iter = npc.begin();
    //iterate thru the screen
    for(int w = 0; w < width; w++)
    {
        for(int h = 0; h < height; h++)
        {   
            //set the iterator to beginning every time..
            //cause it will just add to the count 
            iter = npc.begin();
            advance(iter, count);
            
            //set the current blocks info empty or a block
            (*iter)->isBlock = level[w][h];
            count++;
        }
    }
}

//return enemies X direction based on [w][h] coord
float Level::getEnemiesDirX(int w, int h)
{
    //set the iterator to the beginning
    iter = npc.begin();
    //formula for converting 2d array to lists index
    //list index = currentWidth * fullHeight + currentHeight
    int index = w * height + h;
    advance(iter, index);
    
    //return the facingDir x component
    return (*iter)->getFacingDirX();

        
}

//return enemies X direction based on [w][h] coord
float Level::getEnemiesDirY(int w, int h)
{
    iter = npc.begin();
    int index = w * height + h;
    advance(iter, index);
    

    return (*iter)->getFacingDirY();

}

//return enemies X pos based on [w][h] coord
float Level::getEnemiesPosX(int w, int h)
{
    iter = npc.begin();
    int index = w * height + h;
    advance(iter, index);
    
    return (*iter)->getX();
}

//return enemies Y pos based on [w][h] coord
float Level::getEnemiesPosY(int w, int h)
{
    iter = npc.begin();
    int index = w * height + h;
    advance(iter, index);
    
    return (*iter)->getY();
}

//empty this block
bool Level::emptyBlock(int x, int y)
{
    //get to the current member
    iter = npc.begin();
    int member = x * height + y;
    advance(iter, member);
    
    //get our id
    int id = (*iter)->getID();

    int count = 0;
    //for all the blocks in the map
    //delete the ones that have the same id
    //for the entire level 
    for(iter = npc.begin(); iter != npc.end(); iter++)
    {
        if((*iter)->getID() == id)
        {
            (*iter)->isBlock = false;
            (*iter)->setID(-1);
            
            //recalc the pos
            //get the width in the 2d array
            int scW = count / height;
            //get the height in the 2d array
            int scH = count % height;
            //set the tile to empty
            level[scW][scH] = TILE_EMPTY;
            //delete the current tile
            drawArea->deleteTile(scW, scH);
        }
        count++;
    }
        
//    //delete the currently hit block
//    drawArea->deleteTile(x, y);
//    int index;

    //switch for our id
//    switch(id)
//    {
//        case 1:
//        {
//            for(int i = 1; i < BLOCK_SIZE; i++)
//            {
//                //in case our id was 1
//                //advance to the next in line block
//                iter = npc.begin();
//                index = (x + i) * height + y;
//                advance(iter, index);
//                //set its isBlock to false since its empty now, and its id
//                (*iter)->isBlock = false;
//                (*iter)->setID(-1);
//                //set the tile to empty
//                level[x + i][y] = TILE_EMPTY;
//                //delete the current tile
//                drawArea->deleteTile(x + i, y);
//            }
//                
//            return true;
//        }
//        case 2:
//        {
//            iter = npc.begin();
//            index = (x - 1) * height + y;
//            advance(iter, index);
//            (*iter)->isBlock = false;
//            (*iter)->setID(-1);
//            level[x - 1][y] = TILE_EMPTY;
//            drawArea->deleteTile(x - 1, y);
//
//            iter = npc.begin();
//            index = (x + 1) * height + y;
//            advance(iter, index);
//            (*iter)->isBlock = false;
//            (*iter)->setID(-1);
//            level[x + 1][y] = TILE_EMPTY;
//            drawArea->deleteTile(x + 1, y);
//            return true;
//        }   
//        case 3:
//        {
//            for(int i = 1; i < BLOCK_SIZE; i++)
//            {
//                iter = npc.begin();
//                index = (x - i) * height + y;
//                advance(iter, index);
//                (*iter)->isBlock = false;
//                (*iter)->setID(-1);
//                level[x - i][y] = TILE_EMPTY;
//                drawArea->deleteTile(x - i, y);
//            }
//            
//            return true;
//        }
//        default:
//        {
//            return false;
//        }
//    }

}

//return the current blocks normal for bouncing
vector Level::getEnemiesNormal(int w, int h)
{
    iter = npc.begin();
    int index = w * height + h;
    advance(iter, index);
    
    vector ballPrevPos;
    ballPrevPos.x = (int)(ball->getX() - ball->getFacingDir().x);
    ballPrevPos.y = (int)(ball->getY() - ball->getFacingDir().y);
    vector tmp;
    
    //if ball is hitting the block from left
    if(ballPrevPos.x < (*iter)->getX())
    {
        //if above
        if(ballPrevPos.y < (*iter)->getY())
        {
            //if we have no up neighbours
            if(getBlockUp(w, h) == 0)
            {
                
                tmp.x = 0;
                tmp.y = -1;
                    
                return tmp;
            }
            else
            {
                tmp.x = -1;
                tmp.y = 0;
                    
                return tmp;
            }
        }
        //if below
        else if(ballPrevPos.y > (*iter)->getY())
        {
            //if we dont have any neighbours down
            if(getBlockDown(w, h) == 0)
            {
                tmp.x = 0;
                tmp.y = 1;
                
                return tmp;   
            }
            else
            {
                tmp.x = -1;
                tmp.y = 0;
                
                return tmp;
            }
        }
        //or at the same y pos
        else
        {
            tmp.x = -1;
            tmp.y = 0;
            
            return tmp;
        }
    }
    //else if its hitting from the right
    else if(ballPrevPos.x > (*iter)->getX())
    {
        //if above
        if(ballPrevPos.y < (*iter)->getY())
        {
            //if we have no up neighbours
            if(getBlockUp(w, h) == 0)
            {
                
                tmp.x = 0;
                tmp.y = -1;
                    
                return tmp;
            }
            else
            {
                tmp.x = 1;
                tmp.y = 0;
                    
                return tmp;
            }
        }
        //if below
        else if(ballPrevPos.y > (*iter)->getY())
        {
            if(getBlockDown(w, h) == 0)
            {
                tmp.x = 0;
                tmp.y = 1;
                
                return tmp;   
            }
            else
            {
                tmp.x = 1;
                tmp.y = 0;
                
                return tmp;
            }
        }
        //or the same y pos
        else
        {
            tmp.x = 1;
            tmp.y = 0;
            
            return tmp;
        }
    } 
    //if we are at the same pos x
    else if(ballPrevPos.x == (*iter)->getX())
    {
        //if below
        if(ballPrevPos.y > (*iter)->getY())
        {
            tmp.x = 0;
            tmp.y = 1;
            
            return tmp;
        }
        //if above
        else
        {
            tmp.x = 0;
            tmp.y = -1;
            
            return tmp;
        }
    }
}

//see if there is a block left of current blocks coord
int Level::getBlockLeft(int w, int h)
{
    iter = npc.begin();
    int curIndex = w * height + h;
    advance(iter, curIndex);
    
    int curId = (*iter)->getID();
    int counter = 0;
    
    while(counter < BLOCK_SIZE)
    {
        //see if there is anything left of us
        iter = npc.begin();
        int index = (w - (counter + 1)) * height + h;
        advance(iter, index);

        //if the block on our left is empty space
        if((*iter)->getID() == -1)
        {
            if(counter > 0)
            {
                return counter;
            }
            else
            {
                return -1;
            }
        }
        //if its the same id as we are
        else if((*iter)->getID() == curId)
        {
            counter++;
        }
        else
        {
            if(counter > 0)
            {
                return counter;
            }
            else
            {
                return counter =  BLOCK_SIZE + 1;
            }
        }
    }
}
//see if there is a block right of current blocks coord
int Level::getBlockRight(int w, int h)
{
    iter = npc.begin();
    int curIndex = w * height + h;
    advance(iter, curIndex);
    
    int curId = (*iter)->getID();
    int counter = 0;
    
    while(counter < BLOCK_SIZE)
    {
        //see if there is anything right of us
        iter = npc.begin();
        int index = (w + (counter + 1)) * height + h;
        advance(iter, index);

        //if the block on our left is empty space
        if((*iter)->getID() == -1)
        {
            if(counter > 0)
            {
                return counter;
            }
            else
            {
                return -1;
            }
        }
        //if its the same id as we are
        else if((*iter)->getID() == curId)
        {
            counter++;
        }
        else
        {
            if(counter > 0)
            {
                return counter;
            }
            else
            {
                return counter =  BLOCK_SIZE + 1;
            }
        }
    }
}
//if there is a block under us
int Level::getBlockDown(int w, int h)
{
    iter = npc.begin();
    int curIndex = w * height + (h + 1);
    advance(iter, curIndex);
    
    if((*iter)->isBlock)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//if there is a block above us
int Level::getBlockUp(int w, int h)
{
    iter = npc.begin();
    int curIndex = w * height + (h - 1);
    advance(iter, curIndex);

    if((*iter)->isBlock)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
