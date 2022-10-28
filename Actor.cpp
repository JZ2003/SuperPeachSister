#include "Actor.h"
#include "StudentWorld.h"

Actor :: Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool isAlive, StudentWorld* theWorld)
:GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_World = theWorld;
    m_isAlive = isAlive;
}

Actor :: ~Actor(){
    /*dummy*/
}

Peach :: Peach(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Actor(IID_PEACH, startX, startY, dir, depth, size, true, theWorld){
    m_hitPoint = 1;
    remaining_jump_distance = 0; //To determing the jump status
    m_jumpPower = false;
    m_shootPower = false;
    m_starPower = false; // Peach has no special powers by default
    time_to_recharge_before_next_fire = 0;
    temporaryInvincibility = 0; // Peach has no temporary invincibility either by default
}

Peach :: ~Peach(){
    /*dummy*/
}

void Peach::beBonked(){
    /*dummy*//*重要待完成*/ /*DONE*/
    if(hasStarPowerOrNot() || temporaryInvincibility > 0){
        return; //If peach has either invincibility power, bonking does her nothing bad
    }
    else{
        m_hitPoint --;
        temporaryInvincibility = 10; // give peach temporary invincibility for 10 ticks
        turnOffJumpPower();
        turnOffShootPower(); //Lose her power
        if(m_hitPoint == 1){
            getStudentWorld()->playSound(SOUND_PLAYER_HURT); //If peach still has one hp left
        }
        if(m_hitPoint == 0){
            getKilled(); //If peach has no hp left. DIE
        }
    }
}

void Peach::beDamaged(){
    /*重要待完成*/ /*DONE*/
    beBonked(); //Do exactly the same thing as being bonked
}

void Peach::doSomething(){
    if(isAlive()){
        if(hasStarPowerOrNot()){
            starPowerTime --;
        }
        if(starPowerTime == 0){
            turnOffStarPower(); //If star power time is over, lose the power
        }

        if(temporaryInvincibility > 0){
            temporaryInvincibility --;
        }
        
        if(time_to_recharge_before_next_fire > 0){
            time_to_recharge_before_next_fire --; //If fire is in recharge mode, decrease the recharge time needed
        }
        if(hasStarPowerOrNot()){
            if(getStudentWorld()->bonk(getX(), getY()) || getStudentWorld()->bonk(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->bonk(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->bonk(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
                ; //If peach has star power, try to bonk stuff in four of her corner points
            }
        }
        if(remaining_jump_distance > 0){
            if(getStudentWorld()->isBlockingObject(getX(), getY()+SPRITE_HEIGHT+3)|| getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH-1,getY()+SPRITE_HEIGHT+3)){
                if(!getStudentWorld()->bonk(getX(), getY()+SPRITE_HEIGHT+3)){
                    getStudentWorld()->bonk(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT+3); //If blocked from moving upwards, try to bonk two upper corners
                }
                remaining_jump_distance = 0; //Cancel the jump
            }
            else{
                moveTo(getX(), getY()+4); //Able to move upwards, then move
                remaining_jump_distance --;
            }
        }
        else{
            if(!getStudentWorld()->isBlockingObject(getX(), getY()-1) &&!getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH-1, getY()-1)){
                moveTo(getX(), getY()-4); //If there is no platform under peach, peach will drop due to gravity.
            }
        }
        
        int keyValue; // Get the key from world
        getStudentWorld()->getKey(keyValue);
        if(keyValue == KEY_PRESS_LEFT){
            setDirection(180);
            if(getStudentWorld()->isBlockingObject(getX()-4, getY()) || getStudentWorld()->isBlockingObject(getX()-4, getY()+SPRITE_HEIGHT-1)){ //注意：需要想一   下要不要y+height再判断一遍 /*需要！DONE*/ //Determine first if there is block on two left corner points
                /*完成：DETERMINE DISTANCE AND BONK*/
                if(!getStudentWorld()->bonk(getX()-4, getY())){
                    getStudentWorld()->bonk(getX()-4, getY()+SPRITE_HEIGHT-1); //If there is, bonk it.
                }
            }
            else{
                moveTo(getX()-4, getY()); //If ther isn't, move left.
            }
        }
        if(keyValue == KEY_PRESS_RIGHT){
            setDirection(0);
            if(getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()) || getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT-1)){ //注意：需要想一下要不要y+height再判断一遍 /*需要！DONE*/
                /*完成：DETERMINE DISTANCE AND BONK*/
                if(!getStudentWorld()->bonk(getX()+SPRITE_WIDTH, getY())){
                    getStudentWorld()->bonk(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT-1);
                }
            }
            else{
                moveTo(getX()+4, getY());
            } //Exactly the same thing as moving left
        }
        if(keyValue == KEY_PRESS_UP){
            if(getStudentWorld()->isBlockingObject(getX(), getY()-1)||getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH-1, getY()-1)){ //Jump must be initiated on a physical platform
                if(hasJumpPowerOrNot()){
                    remaining_jump_distance = 12; //If peach has jump power, she'll jump higher
                }
                else{
                    remaining_jump_distance = 8;
                }
                getStudentWorld()->playSound(SOUND_PLAYER_JUMP);
            }
        }
        if(keyValue == KEY_PRESS_SPACE){
            if(hasShootPowerOrNot() && time_to_recharge_before_next_fire == 0){ //Fire could be initialized only if peach has shoot power and the shoot is ready to go
                getStudentWorld()->playSound(SOUND_PLAYER_FIRE);
                time_to_recharge_before_next_fire = 8; //Enter recharge mode
                if(getDirection() == 0){
                    getStudentWorld()->introduceAThing(4, getX()+4, getY(), getDirection()); //Introduce a peach fireball
                }
                else{
                    getStudentWorld()->introduceAThing(4, getX()-4, getY(), getDirection());
                }
            }
        }
    }
    else{
        return; //Peach is dead sadly, so do nothing.
    }
}

BlockingObject :: BlockingObject(int IID, int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld)
:Actor(IID, startX, startY, dir, depth, size, true, theWorld){
}

BlockingObject :: ~BlockingObject()
{}

Block::Block(int startX, int startY,int dir, int depth, double size, int typeOfGoodie, StudentWorld* theWorld)
:BlockingObject(IID_BLOCK, startX, startY, dir, depth, size, theWorld){
    m_typeOfGoodie = typeOfGoodie; // use int to represent goodie type: 0 is nothing; 1 is star; 2 is flower; 3 is mushroom
}

Block :: ~Block(){/*dummy*/}

void Block::beBonked(){
    if(whatTypeOfGoodie() == 0){
        getStudentWorld()->playSound(SOUND_PLAYER_BONK); //This is a boring block bonked
    }
    else if(whatTypeOfGoodie() == 1){
        getStudentWorld()->playSound(SOUND_POWERUP_APPEARS);
        /*待完成：SPAWN A STAR*/ /*DONE*/
        getStudentWorld()->introduceAThing(1, getX(), getY()+8,0);
        changeTypeOfGoodie(0); //Now this block hides no goodie
    }
    else if(whatTypeOfGoodie() == 2){
        getStudentWorld()->playSound(SOUND_POWERUP_APPEARS);
        /*待完成：SPAWN A FLOWER*/ /*DONE*/
        getStudentWorld()->introduceAThing(2, getX(), getY()+8,0);
        changeTypeOfGoodie(0);//Now this block hides no goodie
    }
    else if(whatTypeOfGoodie() == 3){
        getStudentWorld()->playSound(SOUND_POWERUP_APPEARS);
        /*待完成：SPAWN A MUSHROOM*/ /*DONE*/
        getStudentWorld()->introduceAThing(3, getX(), getY()+8,0);
        changeTypeOfGoodie(0);//Now this block hides no goodie
    }
}

void Block::doSomething(){return;} //Block is very boring

Pipe::Pipe(int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld)
:BlockingObject(IID_PIPE, startX, startY, dir, depth, size, theWorld){/*dummy*/}

Pipe :: ~Pipe(){}

void Pipe::beBonked(){
    getStudentWorld()->playSound(SOUND_PLAYER_BONK);
}

void Pipe::doSomething(){return;}

Portal:: Portal(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld)
:Actor(IID, startX, startY, dir, depth, size, true, theWorld){}

Portal::~Portal(){}

Flag::Flag(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Portal(IID_FLAG, startX, startY, dir, depth, size, theWorld){}

Flag::~Flag(){}

void Flag::doSomething(){
    if(isAlive()){ //这个很他妈逼恶心的大if需要额外检查
        if(getStudentWorld()->isPeach(getX(), getY()) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->isPeach(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
            getStudentWorld()->increaseScore(1000); //Peach now finishes this level
            getKilled();
            /*待完成：increase the level*/ /*DONE*/
            getStudentWorld()->reachedFlag(); //Tell the world that the level is finished
        }
    }
    else{
        return;
    }
}
Mario::Mario(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Portal(IID_MARIO, startX, startY, dir, depth, size, theWorld){}

Mario::~Mario(){}

void Mario::doSomething(){
    if(isAlive()){ //这个很他妈逼恶心的大if需要额外检查
        if(getStudentWorld()->isPeach(getX(), getY()) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->isPeach(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
            getStudentWorld()->increaseScore(1000); //Peach now finishes this game
            getKilled();
            /*待完成：increase the level*/ /*DONE*/
            getStudentWorld()->reachedMario(); //Tell the world that the game is finished
        }
    }
    else{
        return;
    }
}

Goodie::Goodie(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld)
:Actor(IID, startX, startY, dir, depth, size, size, theWorld)
{}

Goodie::~Goodie(){}

void Goodie::doSomething(){
    if(getStudentWorld()->isPeach(getX(), getY()) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->isPeach(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
        boostPeach(); //Specified in each subclass
        getKilled(); //Disapper
        getStudentWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else{
        if(!getStudentWorld()->isBlockingObject(getX(), getY()-1) && !getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH-1, getY()-1)){
            moveTo(getX(), getY()-2); //If no platform is under the goodie, drop as graviy dictates
        }
        if(getDirection() == 0){
            if(getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()) || getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT-1)){
                setDirection(180); //If there is block in front, switch direction
                return;
            }
            else{
                moveTo(getX()+2, getY());
            }
        }
        else{
            if(getStudentWorld()->isBlockingObject(getX()-2, getY()) || getStudentWorld()->isBlockingObject(getX()-2, getY()+SPRITE_HEIGHT-1)){
                setDirection(0); //If there is block in front, switch direction
                return;
            }
            else{
                moveTo(getX()-2, getY());
            }
        }
    }
}

Flower::Flower(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Goodie(IID_FLOWER, startX, startY, dir, depth, size, theWorld){}

Flower::~Flower(){}

void Flower::boostPeach(){
    getStudentWorld()->increaseScore(50);
    getStudentWorld()->empowerPeach(2);/*待完成：inform peach that she has shootpower*/ /*DONE*/
    getStudentWorld()->makePeachHitPointToTwo();//Set pitch's hp to two
}

Mushroom::Mushroom(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Goodie(IID_MUSHROOM, startX, startY, dir, depth, size, theWorld){}

Mushroom::~Mushroom(){}

void Mushroom::boostPeach(){
    getStudentWorld()->increaseScore(75);
    getStudentWorld()->empowerPeach(3);/*待完成：inform peach that she has jump power*/ /*DONE*/
    getStudentWorld()->makePeachHitPointToTwo();//Set pitch's hp to two
}

Star::~Star(){}

Star::Star(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Goodie(IID_STAR, startX, startY, dir, depth, size, theWorld){}

void Star::boostPeach(){
    getStudentWorld()->increaseScore(100);
    getStudentWorld()->empowerPeach(1);/*待完成：inform peach that she is invincible*/ /*DONE*/
}

Projectile::Projectile(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld)
:Actor(IID, startX, startY, dir, depth, size, true, theWorld){}

Projectile::~Projectile(){}

void Projectile::doSomething(){
    if(!willDamagePeachOrNot()){ //Use this private helper function to determine if this projectile damage peach or damage enemies
        if(getStudentWorld()->damage(getX(), getY()) || getStudentWorld()->damage(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->damage(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->damage(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
            /*待完成: DETERMINE IF IT OVERLAPS WITH DAMAGEABLE OBJECTS*/ /*DONE*/
            getKilled();
            return;
        }
    }
    else{
        /*待完成：PIRANHAFIRE ATTACT*//*DONE*/
        if(getStudentWorld()->isPeach(getX(), getY()) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->isPeach(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
            getStudentWorld()->damagePeach(); //If overlaps with peach, do damage
            getKilled();
            return;
        }
    }
    if(!getStudentWorld()->isBlockingObject(getX(), getY()-1) && !getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH-1, getY()-1)){
        moveTo(getX(), getY()-2); //Drop if no support
    }
    if(getDirection() == 0){
        if(getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()) || getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT-1)){
            getKilled(); //disapper if running into a wall
            return;
        }
        else{
            moveTo(getX()+2, getY());
        }
    }
    else{
        if(getStudentWorld()->isBlockingObject(getX()-2, getY()) || getStudentWorld()->isBlockingObject(getX()-2, getY()+SPRITE_HEIGHT-1)){
            getKilled();//disapper if running into a wall
            return;
        }
        else{
            moveTo(getX()-2, getY());
        }
    }
}

PeachFireball::PeachFireball(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Projectile(IID_PEACH_FIRE, startX, startY, dir, depth, size, theWorld){}

PeachFireball::~PeachFireball(){}

Shell::Shell(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Projectile(IID_SHELL, startX, startY, dir, depth, size, theWorld){}

Shell::~Shell(){}

PiranhaFireball::PiranhaFireball(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Projectile(IID_PIRANHA_FIRE, startX, startY, dir, depth, size, theWorld){}

PiranhaFireball::~PiranhaFireball(){}

Enemies::Enemies(int IID,int startX, int startY,int dir, int depth, double size, StudentWorld* theWorld)
:Actor(IID, startX, startY, dir, depth, size, true, theWorld){}

Enemies::~Enemies(){}

void Enemies::doSomething(){
    if(!isAlive()){
        return; //If not alive, then do nothing
    }
    if(needToCycleImageOrNot()){ //Use this private helper function to determine if this is a piranha
        increaseAnimationNumber();
    }
    if(getStudentWorld()->isPeach(getX(), getY()) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()) || getStudentWorld()->isPeach(getX(), getY()+SPRITE_HEIGHT-1) || getStudentWorld()->isPeach(getX()+SPRITE_WIDTH-1, getY()+SPRITE_HEIGHT-1)){
        /*待完成：Bonk Peach*/ /*DONE*/
        getStudentWorld()->bonkPeach(); //If overlaps with peach, than bonk her
        return;
    }
    enemyDoItsOwnLittleThang(); //Call each enemy type's distinct function to perfrom their distinct behavior
}
    
void Enemies::beDamaged(){
    getStudentWorld()->increaseScore(100); //Award the player points for defeating enemies
    if(willSpawnAShellOrNot()){
        getStudentWorld()->introduceAThing(5, getX(), getY(), getDirection()); //If this is koopa being damaged, it will spawn a shell
    }
    getKilled();
}
void Enemies::beBonked(){
    getStudentWorld()->playSound(SOUND_PLAYER_KICK);
    getStudentWorld()->increaseScore(100);//Award the player points for defeating enemies
    if(willSpawnAShellOrNot()){
        getStudentWorld()->introduceAThing(5, getX(), getY(), getDirection());  //If this is koopa being damaged, it will spawn a shell
    }
    getKilled();

}
Koopa::Koopa(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Enemies(IID_KOOPA, startX, startY, dir, depth, size, theWorld){}

Koopa::~Koopa(){}

void Koopa::enemyDoItsOwnLittleThang(){
    if(getDirection() == 0){
        if(getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()) || getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT-1)){
            setDirection(180); //If there is blocking object in this direction, then switch diretion
        }
        else{
            if(!getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()-1)){
                setDirection(180); //If there is a gap forward, also switch direction
            }
            else{
                moveTo(getX()+1, getY());
            }
        }
    }
    else{ //Same thing applies for the other direction
        if(getStudentWorld()->isBlockingObject(getX()-1, getY()) || getStudentWorld()->isBlockingObject(getX()-1, getY()+SPRITE_HEIGHT-1)){
            setDirection(0);
        }
        else{
            if(!getStudentWorld()->isBlockingObject(getX()-1, getY()-1)){
                setDirection(0);
            }
            else{
                moveTo(getX()-1, getY());
            }
        }
    }
}

Goomba::Goomba(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Enemies(IID_GOOMBA, startX, startY, dir, depth, size, theWorld){}

Goomba::~Goomba(){}

void Goomba::enemyDoItsOwnLittleThang(){
    if(getDirection() == 0){
        if(getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()) || getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()+SPRITE_HEIGHT-1)){
            setDirection(180);
        }
        else{
            if(!getStudentWorld()->isBlockingObject(getX()+SPRITE_WIDTH, getY()-1)){
                setDirection(180);
            }
            else{
                moveTo(getX()+1, getY());
            }
        }
    }
    else{
        if(getStudentWorld()->isBlockingObject(getX()-1, getY()) || getStudentWorld()->isBlockingObject(getX()-1, getY()+SPRITE_HEIGHT-1)){
            setDirection(0);
        }
        else{
            if(!getStudentWorld()->isBlockingObject(getX()-1, getY()-1)){
                setDirection(0);
            }
            else{
                moveTo(getX()-1, getY());
            }
        }
    }
}

Piranha::Piranha(int startX, int startY, int dir, int depth, double size, StudentWorld* theWorld)
:Enemies(IID_PIRANHA, startX, startY, dir, depth, size, theWorld){
    firingDelay = 0;
}

Piranha::~Piranha(){}

void Piranha::enemyDoItsOwnLittleThang(){
    /*待完成：FIRE*//*DONE*/
    if((getY()-1.5*SPRITE_HEIGHT)>getStudentWorld()->peachYCoord() || (getY()+1.5*SPRITE_HEIGHT)<getStudentWorld()->peachYCoord()){
        return; //If peach is not on the same height level, piranha won't do anything
    }
    if(getStudentWorld()->peachXCoord() <= getX()){
        setDirection(180); //Face towards peach
    }
    else{
        setDirection(0); //Face towards peach
    }
    if(firingDelay > 0){
        firingDelay --; //If piranha just fired, it should wait till the recharge time is up to fire again
        return;
    }
    /*待完成：判断peach的距离并且introduce fireball*//*DONE*/
    if(getStudentWorld()->peachXCoord() > getX()-8*SPRITE_WIDTH && getStudentWorld()->peachXCoord() < getX()+8*SPRITE_WIDTH){
        getStudentWorld()->introduceAThing(6, getX(), getY(), getDirection()); //If peach is getting close enough, then shoot.
        getStudentWorld()->playSound(SOUND_PIRANHA_FIRE);
        firingDelay = 40;
    }
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
