#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject{
public:
    Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool isAlive, StudentWorld* theWorld);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual void beBonked() = 0;
    virtual void beDamaged() = 0;
    virtual bool isBlockingOthers() = 0;
    virtual bool isDamageable() = 0;
    bool isAlive() const {return m_isAlive;}
    void getKilled(){m_isAlive = false;}
    StudentWorld* getStudentWorld() const {return m_World;}
private:
    StudentWorld* m_World; //every actor should know the world it exists
    bool m_isAlive; //every actor should has a life status
};

class Peach : public Actor{
public:
    Peach(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Peach();
    virtual void doSomething();
    virtual void beBonked();
    virtual void beDamaged();
    virtual bool isBlockingOthers(){return false;}
    virtual bool isDamageable(){return true;}
    void makeHitPointToTwo(){m_hitPoint = 2;}
    void turnOnJumpPower(){m_jumpPower = true;}
    void turnOnShootPower(){m_shootPower = true;}
    void turnOnStarPower(){
        m_starPower = true;
        starPowerTime = 150;
    }
    void turnOffJumpPower(){m_jumpPower = false;}
    void turnOffShootPower(){m_shootPower = false;}
    void turnOffStarPower(){m_starPower = false;}
    bool hasJumpPowerOrNot() const{return m_jumpPower;}
    bool hasShootPowerOrNot() const{return m_shootPower;}
    bool hasStarPowerOrNot() const{return m_starPower;}
private:
    int m_hitPoint;
    int remaining_jump_distance;
    bool m_starPower;
    bool m_shootPower;
    bool m_jumpPower;
    int time_to_recharge_before_next_fire;
    int temporaryInvincibility;
    int starPowerTime;
};

class BlockingObject : public Actor{
public:
    BlockingObject(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~BlockingObject();
    virtual void beDamaged(){/*DONOTHING*/}
    virtual bool isBlockingOthers(){return true;}
    virtual bool isDamageable(){return false;}
};

class Block: public BlockingObject{
public:
    Block(int startX, int startY, int dir, int depth, double size, int typeOfGoodie, StudentWorld* theWorld);
    virtual ~Block();
    virtual void doSomething();
    virtual void beBonked();
    int whatTypeOfGoodie() const {return m_typeOfGoodie;}
private:
    void changeTypeOfGoodie(int x){m_typeOfGoodie = x;}
    int m_typeOfGoodie; //Block object might hold a kind of goodie
};

class Pipe: public BlockingObject{
public:
    Pipe(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Pipe();
    virtual void doSomething();
    virtual void beBonked();
};

class Portal : public Actor{
public:
    Portal(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Portal();
    virtual bool isBlockingOthers(){return false;}
    virtual bool isDamageable() {return false;}
    virtual void beDamaged(){/*DONOTHING*/}
    virtual void beBonked(){/*DONOTHING*/}
};

class Flag : public Portal{
public:
    Flag(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Flag();
    virtual void doSomething();
};

class Mario : public Portal{
public:
    Mario(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Mario();
    virtual void doSomething();
};

class Goodie : public Actor{
public:
    Goodie(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Goodie();
    virtual void doSomething();
    virtual bool isBlockingOthers(){return false;}
    virtual bool isDamageable() {return false;}
    virtual void beBonked(){/*DONOTHING*/}
    virtual void beDamaged(){/*DONOTHING*/}
private:
    virtual void  boostPeach() = 0; //Different goodie boosts peach differently
};

class Flower : public Goodie{
public:
    Flower(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    ~Flower();
private:
    void boostPeach();
};

class Mushroom : public Goodie{
public:
    Mushroom(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    ~Mushroom();
private:
    void boostPeach();
};

class Star : public Goodie{
public:
    Star(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    ~Star();
private:
    void boostPeach();
};

class Projectile : public Actor{
public:
    Projectile(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld);
    ~Projectile();
    virtual bool isBlockingOthers(){return false;}
    virtual bool isDamageable() {return false;}
    virtual void beDamaged(){/*DONOTHING*/}
    virtual void beBonked(){/*DONOTHING*/}
    virtual void doSomething();
private:
    virtual bool willDamagePeachOrNot() = 0; //This helper private function determines the projectile hurts peach or enemies
};

class PeachFireball : public Projectile{
public:
    PeachFireball(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~PeachFireball();
private:
    virtual bool willDamagePeachOrNot(){return false;}
};

class Shell : public Projectile{
public:
    Shell(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Shell();
private:
    virtual bool willDamagePeachOrNot(){return false;}
};

class PiranhaFireball : public Projectile{
public:
    PiranhaFireball(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~PiranhaFireball();
private:
    virtual bool willDamagePeachOrNot(){return true;}
};


class Enemies : public Actor{
public:
    Enemies(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld);
    ~Enemies();
    virtual void beBonked();
    virtual void beDamaged();
    virtual bool isBlockingOthers(){return false;}
    virtual bool isDamageable() {return true;}
    virtual void doSomething();
private:
    virtual bool needToCycleImageOrNot() = 0; //Determine if this is piranha
    virtual bool willSpawnAShellOrNot() = 0;//Determine if this is koopa
    virtual void enemyDoItsOwnLittleThang() = 0; //Each enemy type has its own specific thing to do
};

class Goomba : public Enemies{
public:
    Goomba(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Goomba();
private:
    virtual bool needToCycleImageOrNot(){return false;}
    virtual void enemyDoItsOwnLittleThang();
    virtual bool willSpawnAShellOrNot(){return false;}
};

class Koopa : public Enemies{
public:
    Koopa(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Koopa();
private:
    virtual bool needToCycleImageOrNot(){return false;}
    virtual void enemyDoItsOwnLittleThang();
    virtual bool willSpawnAShellOrNot(){return true;}
};

class Piranha : public Enemies{
public:
    Piranha(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld);
    virtual ~Piranha();
private:
    virtual bool needToCycleImageOrNot(){return true;}
    virtual void enemyDoItsOwnLittleThang();
    virtual bool willSpawnAShellOrNot(){return false;}
    int firingDelay;
};


#endif // ACTOR_H_

