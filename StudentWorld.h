#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>
//#include "Actor.h" //Include the Actors explicitly


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isPeach(int x, int y);
    int peachXCoord();
    int peachYCoord();
    bool isBlockingObject(int x, int y);
    void reachedFlag(){m_reachedFlag = true;}
    bool hasReachedFlagOrNot(){return m_reachedFlag;}
    void reachedMario(){m_reachedMario = true;}
    bool hasReachedMarioOrNot(){return m_reachedMario;}
    void makePeachHitPointToTwo();
    void empowerPeach(int x);
    void introduceAThing(int type, int x, int y, int dir);
    void bonkPeach();
    void damagePeach();
    bool bonk(int x, int y);
    bool damage(int x, int y);
private:
    Peach* m_Peach;
    std::list<Actor*> m_list;
    bool m_reachedFlag; //Use a bool member variable to indicate if the flag is reached
    bool m_reachedMario; //Use a bool member variable to indicate if the mario is reached
};

#endif // STUDENTWORLD_H_
