
#include "pongplayer.h"

Pongplayer::Pongplayer(DrawEngine* de, Level* lvl, int index, float x, float y, 
            int numlives, char left_key, char right_key, char spell) : 
            Character(de, lvl, index, x, y, numlives, left_key, right_key)
{
    spellKey = spell;
    ball = NULL;
    classID = PONGPLAYER_CLASSID;

}
//catch users input key
bool Pongplayer::keyPress(char c)
{
    //catch the users input key press
    bool isKey = Character::keyPress(c);
    //if that key is not defined in the char class
    if(!isKey)
    {
        //check if that key may be the spell key and check if the ball hasnt moved yet
        if(c == spellKey && ball->getBallStatus() == false)
        {
            //if its a spell key run the castSpell method
            castSpell();
            return true;
        }
        return false;
    }
    return isKey;
}
//set the ball players ptr
void Pongplayer::setBall(Ball* b)
{
    ball = b;
}

//if we have hit space then make the ball move
void Pongplayer::castSpell()
{
    ball->setBallOn(true);
}