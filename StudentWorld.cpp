#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

bool StudentWorld::isPeach(int x, int y){ //需要额外检查
    int actorX = m_Peach->getX();
    int actorY = m_Peach->getY();
    if(actorX <= x && actorX+SPRITE_WIDTH > x){
        if(actorY <= y && actorY+SPRITE_HEIGHT > y){
            return true; //Determine if the pixel passed in is also occupied by the peach object
        }
    }
    return false;
}

bool StudentWorld::isBlockingObject(int x, int y){
    for(list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); it ++){
        int actorX = (*it)->getX();
        int actorY = (*it)->getY();
        if(actorX <= x && actorX+SPRITE_WIDTH > x){
            if(actorY <= y && actorY+SPRITE_HEIGHT > y){
                if((*it)->isBlockingOthers()) //Determine if the pixel passed in is also occupied by any blocking object
                    return true;
            }
        }
    }
    return false;
}

bool StudentWorld::bonk(int x, int y){
    for(list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); it ++){
        int actorX = (*it)->getX();
        int actorY = (*it)->getY();
        if(actorX <= x && actorX+SPRITE_WIDTH > x){
            if(actorY <= y && actorY+SPRITE_HEIGHT > y){
                (*it)->beBonked(); //Bonk the object at the pixel we pass in
                return true;
            }
        }
    }
    return false; //If there is nothing that could be bonked, then return false to indicate bonking fails
}

bool StudentWorld::damage(int x, int y){
    for(list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); it ++){
        int actorX = (*it)->getX();
        int actorY = (*it)->getY();
        if(actorX <= x && actorX+SPRITE_WIDTH > x){
            if(actorY <= y && actorY+SPRITE_HEIGHT > y){
                if((*it)->isDamageable() && (*it)->isAlive()){ //Only object that is alive could be damage
                    (*it)->beDamaged(); //Damage the object at the pixel we pass in
                    return true;
                }
            }
        }
    }
    return false;//If there is nothing that could be damaged, then return false to indicate bonking fails
}

void StudentWorld::makePeachHitPointToTwo(){
    m_Peach->makeHitPointToTwo();
}

int StudentWorld :: peachXCoord() {return m_Peach->getX();}

int StudentWorld :: peachYCoord() {return m_Peach->getY();}

void StudentWorld::bonkPeach(){m_Peach->beBonked();}

void StudentWorld::damagePeach(){m_Peach->beDamaged();}

void StudentWorld::empowerPeach(int x){
    /*dummy*/
    /*待完成：如何设计empower*/
    if(x == 1){
        m_Peach->turnOnStarPower();
    }
    if(x == 2){
        m_Peach->turnOnShootPower();
    }
    if(x == 3){
        m_Peach->turnOnJumpPower();
    }
    /*Use different indexes to indicate the way to empower peach*/
}

void StudentWorld::introduceAThing(int type, int x, int y, int dir){
    if(type == 1){
        m_list.push_back(new Star(x,y,dir,1,1,this));
    }
    if(type == 2){
        m_list.push_back(new Flower(x,y,dir,1,1,this));
    }
    if(type == 3){
        m_list.push_back(new Mushroom(x,y,dir,1,1,this));
    }
    if(type == 4){
        m_list.push_back(new PeachFireball(x,y,dir,1,1,this));
    }
    if(type == 5){
        m_list.push_back(new Shell(x,y,dir,1,1,this));
    }
    if(type == 6){
        m_list.push_back(new PiranhaFireball(x,y,dir,1,1,this));
    }
    /*Use different indexes to indicate what objects to introduce to the world*/
}




StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_Peach = nullptr;
}



int StudentWorld::init()
{
    m_reachedFlag = false;
    m_reachedMario = false;
    Level lev(assetPath());
    int levelNum = getLevel();
    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << levelNum << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found){
        cerr << "Could not find level01.txt data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format){
        cerr << "level01.txt is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if(result == Level::load_success){
        for(int i = 0; i < GRID_WIDTH; i ++){
            for(int j = 0; j < GRID_HEIGHT; j ++){
                Level::GridEntry ge;
                ge = lev.getContentsOf(i, j);
                int i1 = randInt(1, 180);
                int i2 = randInt(1, 180);
                int i3 = randInt(1, 180);
                switch (ge){
                    case Level::peach:
                        m_Peach = new Peach(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 0, 1, this);
                        break;
                    case Level::block:
                        m_list.push_back(new Block(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,0,2,1,0,this));
                        break;
                    case Level::pipe:
                        m_list.push_back(new Pipe( i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 2, 1, this));
                        break;
                    case Level::star_goodie_block:
                        m_list.push_back(new Block( i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 2, 1,1, this));
                        break;
                    case Level::mushroom_goodie_block:
                        m_list.push_back(new Block( i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 2, 1,3, this));
                        break;
                    case Level::flower_goodie_block:
                        m_list.push_back(new Block( i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 2, 1, 2, this));
                        break;
                    case Level::mario:
                        m_list.push_back(new Mario(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,0,1,1,this));
                        break;
                    case Level::flag:
                        m_list.push_back(new Flag(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,0,1,1,this));
                        break;
                    case Level::piranha:
                        if(i1 <= 90)
                        { m_list.push_back(new Piranha(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,0,0,1,this));}
                        else
                        { m_list.push_back(new Piranha(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,180,0,1,this));}
                        break;
                    case Level::koopa:
                        if(i2 <= 90)
                        { m_list.push_back(new Koopa(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,0,0,1,this));}
                        else
                        { m_list.push_back(new Koopa(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,180,0,1,this));}
                        break;
                    case Level::goomba:
                        if(i3 <= 90)
                        { m_list.push_back(new Goomba(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,0,0,1,this));}
                        else
                        { m_list.push_back(new Goomba(i*SPRITE_WIDTH, j*SPRITE_HEIGHT,180,0,1,this));}
                        break;
                    case Level::empty:
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if(m_Peach->isAlive())
        m_Peach->doSomething(); //Give peach a chance to do something
    for(list<Actor*>::iterator it = m_list.begin(); it != m_list.end(); it ++){
        if((*it)->isAlive()){
            (*it)->doSomething(); //Give each actor a chance to do something
        }
        if(!m_Peach->isAlive()){
            playSound(SOUND_PLAYER_DIE); //If peach is no longer alive
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if(hasReachedFlagOrNot()){
            playSound(SOUND_FINISHED_LEVEL); //If peach reach the flag
            return GWSTATUS_FINISHED_LEVEL;
        }
        if(hasReachedMarioOrNot()){
            playSound(SOUND_GAME_OVER); //If peach reach the mario
            return GWSTATUS_PLAYER_WON;
        }
    }
    
    for(list<Actor*>::iterator it = m_list.begin(); it != m_list.end();){
        if(!(*it)->isAlive()){ //Delete every object that is killed in the round
            delete (*it);
            it = m_list.erase(it); //Erase its pointer from the list
        }
        else{
            it ++;
        }
    }
    
    ostringstream oss;
    oss.fill('0');
    oss << "Lives: " << getLives() << "  Levels: "<< setw(2) << getLevel();
    oss << "  Points: " << setw(6) << getScore();
    if(m_Peach->hasJumpPowerOrNot()){
        oss << " JumpPower!";
    }
    if(m_Peach->hasStarPowerOrNot()){
        oss << " StarPower!";
    }
    if(m_Peach->hasShootPowerOrNot()){
        oss << " ShootPower!";
    }
    string stats = oss.str();
    setGameStatText(stats);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //不确定：是否需要判断两个pointers是否被initialize,应该不用 /*不用，已经被nullptr了*/
    delete m_Peach;
    for(list<Actor*>::iterator it = m_list.begin(); it != m_list.end();){
        delete *it;
        it = m_list.erase(it);
    }
}

StudentWorld::~StudentWorld(){
    cleanUp();
}
