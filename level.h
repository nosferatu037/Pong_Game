/* 
 * File:   level.h
 * Author: nosferatu
 *
 * Created on June 20, 2015, 6:29 PM
 */

#ifndef LEVEL_H
#define	LEVEL_H

#include <list>
#include "drawEngine.h"
//#include "sprite.h"


//#include "character.h"
//we could either include the character.h or just make a call to character class
//the reason behind this is cyclical inclusion of header files and errors it produces
class Blocks;
class Character;
class Ball;
class Sprite;

using std::list;

//types of sprites in the level, player(char) enemy or fireball
enum
{
    SPRITE_PLAYER,
    SPRITE_BALL,
};
//types of tiles in the level, empty or block to pop
enum
{
    TILE_EMPTY,
    TILE_BLOCK
};


//level class
class Level
{
public:
    //Level constructor and destructor to initialize and destroy the data
    Level(DrawEngine *de);
    ~Level();
    
    //add a player to the level
    void addPlayer(Character *p);
    //add a ball to the level
    void addBall(Ball *b);
    
    //update the level method
    void update();
     
    //draw the level method
    void draw();
    
    //figure out if key was pressed
    bool keyPress(char c);
    
    //make the Sprite,Ball and Blocks class read the protected and private data members
    //we will need to this so these classes and level can talk to each other
    friend class Sprite;
    friend class Ball;
    friend class Blocks;
    
    //return the level info
    //true for TILE_BLOCK
    //false for TILE_EMPTY
    bool getLevelInfo(int x, int y);
    
    //return the level number/difficulty number
    int getLevel();
    
    //update our enemies facingDirection
    //basically its a normal pointing from the enemy to outter space
    void updateEnemiesDir();
    
    //LAPLACIAN normal direction of enemies
    //update our normal direction of the enemy
    //by sampling the neighbourhood of 8 cells around the current enemy
    void updateDirectionLaplace(int index);
    
    //update our enemies info
    //maybe some blocks have been destroyed, mark them as empty 
    //so we dont collide again
    void updateEnemies();
    struct vector getEnemiesNormal(int w, int h);
    
    //get the enemies/blocks facingDir in XY
    float getEnemiesDirX(int w, int h);
    float getEnemiesDirY(int w, int h);
    
    //get current enemies pos in XY
    float getEnemiesPosX(int w, int h);
    float getEnemiesPosY(int w, int h);
    
    int getBlockLeft(int w, int h);
    int getBlockRight(int w, int h);
    int getBlockUp(int w, int h);
    int getBlockDown(int w, int h);

protected:
    //create level method
    void createLevel();
    //remove our blocks
    bool emptyBlock(int x, int y);
    
    void averageDirections(int x, int y);
    
private:
    //data members width/height
    int width;
    int height;
    
    //2d array of ptrs for our level drawing
    char **level;
    
    //player to be included in the level as well as the ball
    Character *player;
    Ball *ball;
    
    //draw engine ptr so we can draw the level
    DrawEngine *drawArea;
    
    //current level number/difficulty data member
    int currLevel;

public:
    //list data member for non player characters (blocks)
    list<Blocks*> npc;
    //iterator data member for the list of npc
    list<Blocks*>::iterator iter;
};

#endif	/* LEVEL_H */

