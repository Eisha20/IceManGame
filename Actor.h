#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject { // "Base" Class
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* swActor);

    virtual void doSomething() = 0; //pure virtual function
    void setState(bool state);
    bool getState();
    virtual StudentWorld* getWorld();
    virtual ~Actor();

private:
    bool _state;//Actor is alive or dead
    int _level;
    StudentWorld* _swActor;
};


/*************************************************************************************************/
class IceMan : public Actor {
public:
    IceMan(StudentWorld* swIceMan);
    void doSomething();

    int getHealth();
    void decHealth(int health);
    int getSonar();
    void setSonar(int sonar);
    int getWater();
    void setWater(int water);
    int getGoldNuggets();
    void setGoldNuggets(int goldNuggets);
    int getBarrels();
    void setBarrels(int barrelsLeft);
    virtual ~IceMan();

private:

    int _health;
    int _water;
    int _sonar;
    int _goldNuggets;
    int _barrelsLeft;
};

/**************************************************************************************************/
class Ice : public Actor {
public:
    Ice(int startX, int startY, StudentWorld* swIce);
    void doSomething();

    virtual ~Ice();

private:

};

/******************************************PROTESTER HIERARCHY*****************************/

class Protester : public Actor {
public:
    Protester(int image, int hitPoints, StudentWorld* sw_Protester);

    int getHitPoints();
    void setHitPoints(int hitPoints);

    bool getIsLeavingField();
    void setIsLeavingField(bool isLeavingField);
    bool isWithinShoutingDistance();
    bool isAbleToMove();
    bool isAbleToYell();
    virtual void doSomething();

    virtual ~Protester();

private:
    StudentWorld* _swProtester;
    int _ticksToWaitBetweenMoves;
    int _ticksToWaitBetweenYells;
    int _hitPoints;
    int _image;
    int _level;
    int _numSquaresToMoveInCurrentDirection;
    bool _isLeavingField;
};


class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* swRegProtester);




    virtual ~RegularProtester();
private:


};


/*
class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* swR_Protester);

  virtual ~RegularProtester();
private:

}; */

//class HardcoreProtester : public Protester {
//public:
//    HardcoreProtester();
//
//    virtual ~HardcoreProtester();
//private:
//
//};
///*******************************************************************************************/
//
///******************************************GOODIES HIERARCHY****************************/
//
//class Goodie : public Actor {
//public:
//    Goodie();
//
//  virtual ~Goodie();
//private:
//
//};

//class OilBarrel : public Goodies {
//public:
//    OilBarrel();
//  virtual ~OilBarrel();
//private:
//};
//
class Boulder : public Actor {
public:
    Boulder(int x, int y, StudentWorld* swBoulder);
    virtual void doSomething();
    bool isIcePresentBelow(int x, int y);
    void fall();
    virtual ~Boulder();
private:
    bool _stable;
};

//class Sonar : public Goodies {
//public:
//    Sonar();
//
//  virtual ~Sonar();
//private:
//
//};
//
//class GoldNugget : public Goodies {
//public: 
//    GoldNugget();
//    virtual ~GoldNugget();
//private:
//
//};

#endif // ACTOR_H_
